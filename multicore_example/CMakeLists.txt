add_executable(multicore_example
    multicore_example.c
)

target_link_libraries(multicore_example
        pico_multicore
        pico_stdlib
        hardware_adc
)

# create map/bin/hex file etc.
pico_add_extra_outputs(multicore_example)

# add url via pico_set_program_url
example_auto_set_url(multicore_example)