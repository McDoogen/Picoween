add_executable(spinCross
    spinCross.cpp
)

target_link_libraries(spinCross pico_stdlib hardware_adc hardware_pwm)

pico_enable_stdio_usb(spinCross 1)
pico_enable_stdio_uart(spinCross 0)

pico_add_extra_outputs(spinCross)