cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES (VS Code Extension) ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ========================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(serial_led_control C CXX ASM)

pico_sdk_init()

# Adiciona todos os arquivos fonte
add_executable(serial_led_control
    serial_led_control.c
    src/ssd1306.c
    src/ws2812.c
    # Adicione outros arquivos .c aqui (ex: font.c)
)

pico_set_program_name(serial_led_control "serial_led_control")
pico_set_program_version(serial_led_control "0.1")

# Habilita UART e USB
pico_enable_stdio_uart(serial_led_control 1)
pico_enable_stdio_usb(serial_led_control 1)

# Gera o cabeçalho PIO para a WS2812 (ajuste o caminho se necessário)
pico_generate_pio_header(serial_led_control ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Bibliotecas necessárias
target_link_libraries(serial_led_control
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_gpio
    hardware_dma
    hardware_uart
)

# Diretórios de inclusão
target_include_directories(serial_led_control PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/src
)

pico_add_extra_outputs(serial_led_control)