#MBOT
add_executable(mbot
  mbot.c
)
target_link_libraries(mbot
  pico_stdlib
  pico_multicore
  pico_time
  hardware_pwm
  hardware_sync
  hardware_i2c
  hardware_flash
  rclib
  comms
)
pico_enable_stdio_usb(mbot 1)
pico_enable_stdio_uart(mbot 0)
pico_add_extra_outputs(mbot)

#MBOT testing
add_executable(mbot_testing
  mbot_testing.c
)
target_link_libraries(mbot_testing
  pico_stdlib
  pico_multicore
  pico_time
  hardware_pwm
  hardware_sync
  hardware_i2c
  hardware_flash
  rclib
  comms
)
pico_enable_stdio_usb(mbot_testing 1)
pico_enable_stdio_uart(mbot_testing 0)
pico_add_extra_outputs(mbot_testing)
