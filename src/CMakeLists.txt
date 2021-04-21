# What CMake to start at
cmake_minimum_required(VERSION 3.12)
 
# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)


# Set the name and version of the project
project(FlyPicoPi C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Link the Project to a source file (step 4.6)
#add_executable(FlyPicoPi main.cpp font.cc oled.cc splash.cc usb_descriptors.c)
add_executable(FlyPicoPi 
main.cpp 
font.cc 
oled.cc 
splash.cc 

sdlib/crc.c
sdlib/diskio.c  
sdlib/f_util.c 
sdlib/ff_stdio.c 

sdlib/ff.c  
sdlib/ffsystem.c 
sdlib/ffunicode.c  
sdlib/diskio.c  

sdlib/rtc.c 
sdlib/sd_card.c 
sdlib/hw_config.c 
sdlib/sd_spi.c 
)

# Link the Project to an extra library (pico_stdlib)


target_link_libraries(FlyPicoPi PRIVATE
pico_stdlib
pico_audio_i2s
pico_multicore 
pico_util 

hardware_uart
hardware_flash
hardware_i2c
hardware_adc 
hardware_dma 
hardware_spi
hardware_rtc

#tinyusb_device 
#tinyusb_board
)


target_compile_definitions(FlyPicoPi PRIVATE
# compile time configuration of I2S

PICO_AUDIO_I2S_MONO_OUTPUT=0
PICO_AUDIO_I2S_MONO_INPUT=0

CFG_TUD_AUDIO=0
CFG_TUD_MIDI=1
CFG_TUD_MIDI_RX_BUFSIZE=64
CFG_TUD_MIDI_TX_BUFSIZE=64

PICO_SD_CLK_PIN=14
PICO_SD_DATI_PIN=15 
PICO_SD_DAT0_PIN=12 
PICO_SD_CMD_PIN=13 

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


add_custom_target(FlyPicoPiDeploy
    COMMAND cp FlyPicoPi.uf2 /media/mdietric/RPI-RP2
    DEPENDS FlyPicoPi
)

