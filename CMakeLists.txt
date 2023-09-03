cmake_minimum_required(VERSION 3.17)


set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)
project(snakeBox_project)
pico_sdk_init()


add_executable(snakeBox
    snakeBox.cpp
)

target_link_libraries(snakeBox pico_stdlib hardware_i2c)

pico_enable_stdio_usb(snakeBox 1)
pico_enable_stdio_uart(snakeBox 0)
pico_add_extra_outputs(snakeBox)