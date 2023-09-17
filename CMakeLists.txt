cmake_minimum_required(VERSION 3.17)


set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)
project(picoween)
pico_sdk_init()

add_subdirectory(spinCross)