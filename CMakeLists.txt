# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Zenith C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_subdirectory(sd-card-lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

add_executable(Zenith src/main.c src/altimeter.c src/bmp180.c src/state_handler.c src/hw_config.c src/data_logger.c) 

pico_set_program_name(Zenith "Zenith")
pico_set_program_version(Zenith "0.1")

pico_enable_stdio_usb(Zenith 1)

# Add the standard library to the build
target_link_libraries(Zenith 
                        pico_stdlib 
                        hardware_i2c 
                        no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 
                        pico_multicore
                        )

# Add the standard include files to the build
target_include_directories(Zenith PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )


pico_add_extra_outputs(Zenith)

