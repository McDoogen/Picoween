add_executable(spinCross
    spinCross.cpp
)

target_link_libraries(spinCross pico_stdlib)

pico_add_extra_outputs(spinCross)