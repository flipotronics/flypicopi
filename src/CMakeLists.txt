# What CMake to start at
cmake_minimum_required(VERSION 3.12)
 
# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)

include(pico_extras_import.cmake)

# Set the name and version of the project
project(FlyPicoPi C CXX)
 
# Link the Project to a source file (step 4.6)
add_executable(FlyPicoPi main.cpp)
 
# Link the Project to an extra library (pico_stdlib)

target_link_libraries(FlyPicoPi PRIVATE
pico_stdlib
pico_audio_i2s
hardware_uart
hardware_flash
hardware_i2c
)

target_compile_definitions(FlyPicoPi PRIVATE
# compile time configuration of I2S

PICO_AUDIO_I2S_MONO_OUTPUT=0
PICO_AUDIO_I2S_MONO_INPUT=0

#define for our example code
USE_AUDIO_I2S=1
)
 
# Initalise the SDK
pico_sdk_init()
 
# Enable USB, UART output
pico_enable_stdio_usb(FlyPicoPi 1)
pico_enable_stdio_uart(FlyPicoPi 0)
 
# Enable extra outputs (SWD?)
pico_add_extra_outputs(FlyPicoPi)

