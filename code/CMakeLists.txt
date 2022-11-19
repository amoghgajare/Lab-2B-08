cmake_minimum_required(VERSION 3.13) # CMAKE (heading)
include(pico_sdk_import.cmake) # CMAKE -> PICO SDK (include)

project(apds9960_interface C CXX ASM) # CMAKE ("project")
pico_sdk_init() # PICO SDK (initialization)

add_executable(apds9960_interface)

pico_generate_pio_header(apds9960_interface ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)

target_sources(apds9960_interface PRIVATE pio_i2c.c pio_i2c.h apds9960_interface.c pixel.c)

pico_enable_stdio_usb(apds9960_interface 1) # PICO SDK (USB)
pico_enable_stdio_uart(apds9960_interface 0) # PICO SDK (USB)

target_link_libraries(apds9960_interface PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(apds9960_interface)