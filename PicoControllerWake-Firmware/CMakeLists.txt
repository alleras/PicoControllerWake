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
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(PicoControllerWake C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(PicoControllerWake 
    PicoControllerWake.c
    cdc_output.c
    whitelist.c
    bt_scanner.c
    util.c
    pc_waker.c
    usb_descriptors.c
    flash_storage.c
    error_codes.c
)

target_compile_definitions(PicoControllerWake PRIVATE
    DEBUG_LOGGING=$<IF:$<CONFIG:Debug>,1,0>
)

pico_set_program_name(PicoControllerWake "PicoControllerWake")
pico_set_program_version(PicoControllerWake "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(PicoControllerWake 0)
pico_enable_stdio_usb(PicoControllerWake 0)

# Add the standard include files to the build
target_include_directories(PicoControllerWake PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add all required libraries
target_link_libraries(PicoControllerWake
        pico_stdlib
        pico_cyw43_arch_lwip_poll
        pico_btstack_ble
        pico_btstack_classic
        pico_btstack_cyw43
        hardware_flash
        hardware_sync
        tinyusb_device
        tinyusb_board
)

target_compile_definitions(PicoControllerWake PRIVATE
    CYW43_ENABLE_BLUETOOTH=1
)

pico_add_extra_outputs(PicoControllerWake)

