#include <stdio.h>
#include <stdint.h>
#include <rc/motor/motor.h>
#include <rc/encoder/encoder.h>
#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <controls/controller.h>
#define MAIN_LOOP_HZ 50.0

float transformspeed(float speed){
    float radius = 8.3 / 100;
    return speed * radius * PI / 60
}

float l_motor_v_to_d(float vel){
    float a = -0.8793;
    float b = 0.1306;
    float d = a*(-vel)+b;
    return d;
}

float r_motor_v_to_d(float vel){
    float a = -0.8802;
    float b = 0.115;
    float d = a*(-vel)+b;
    return d;
}

float vel = 0.3;
float l_duty = l_motor_v_to_d(vel);
float r_duty = r_motor_v_to_d(vel);
int l_encoder_reading;
float l_wheel_speed;
int r_encoder_reading;
float r_wheel_speed;
float l_wheel_speed_old = 0.0;
float r_wheel_speed_old = 0.0;
float l_wheel_speed_lowpass;
int main() {
    rc_motor_set(1, (int32_t)(l_d*INT_16_MAX)); // left
    rc_motor_set(3, -(int32_t)(r_d*INT_16_MAX)); // right
    l_encoder_reading = -rc_encoder_read_delta(1);
    r_encoder_reading = -rc_encoder_read_delta(3);
    l_wheel_speed = -RPM_conversion_factor * l_encoder_reading * (2*M_PI*r/60.) * MAIN_LOOP_HZ; // convert to m/s
    r_wheel_speed = RPM_conversion_factor * r_encoder_reading * (2*M_PI*r/60.) * MAIN_LOOP_HZ; // convert to m/s

    rc_filter_t rc_pid = rc_filter_empty();
    rc_filter_pid(&rc_pid,
                  0.1,    //kp                                                              
                  0.1,    //ki
                  0.001,  //kd
                  1.0 / 5,
                  1.0 / MAIN_LOOP_HZ);
    rc_filter_t rc_lowpass = rc_filter_empty();
    rc_filter_first_order_lowpass(&rc_lowpass,
                                  1.0 / MAIN_LOOP_HZ
                                  1.0 / 5);
    
    l_wheel_speed_lowpass = rc_filter_march(&rc_lowpass, l_wheel_speed);
    l_wheel_pid_duty = rc_filter_march(&rc_filter_pid, vel - l_wheel_speed);
    rc_motor_set(1, l_wheel_pid_duty);


}
