cmake_minimum_required(VERSION 3.13)

# Generates compile_commands.json located in ../build/compile_commands.json
# It is used by the clangd or cpp extensions in VSCode for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set path to the STM32Cube libraries and toolchain for compilation
set(STM32_TOOLCHAIN_PATH /opt/st/stm32cubeclt_1.18.0/GNU-tools-for-STM32)
set(STM32_CUBE_L0_PATH /home/richard/STM32Cube/Repository/STM32Cube_FW_L0_V1.12.4)
set(STM32_TARGET_TRIPLET arm-none-eabi)

# Path to the cmake used to find all the STM32Cube libraries 
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

# Enables C CPP and ASM 
project(example_interface C ASM CXX)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall)
add_definitions(-Wextra)

find_package(CMSIS COMPONENTS STM32L0 REQUIRED)
find_package(HAL COMPONENTS STM32L0 RCC PCD UART USART GPIO DMA CORTEX SPI TIM I2C FLASH )
add_definitions(-DUSE_FULL_LL_DRIVER)

include_directories(
    include
)

add_executable(main
    src/main.cpp

    src/gpio.c
    src/usart.c
    src/i2c.c 
    src/syscalls.c 

    src/stm32l0xx_hal_msp.c
    src/stm32l0xx_it.c
)

target_compile_options(main PRIVATE -fno-exceptions)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(main PRIVATE -Og -g3 -fdata-sections -ffunction-sections -fno-inline)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(main PRIVATE -O2)
endif()
# prints the memory usage at the end of the linking process 
target_link_options(main PUBLIC -Wl,--print-memory-usage,--undefined=uxTopUsedPriority)

target_link_libraries(main
  HAL::STM32::L0::RCC
  HAL::STM32::L0::RCCEx
  HAL::STM32::L0::DMA
  HAL::STM32::L0::GPIO
  HAL::STM32::L0::CORTEX
  HAL::STM32::L0::UART
  HAL::STM32::L0::USART
  HAL::STM32::L0::SPI
  HAL::STM32::L0::TIM
  HAL::STM32::L0::TIMEx
  HAL::STM32::L0::I2C
  HAL::STM32::L0::I2CEx
  HAL::STM32::L0::FLASH
  HAL::STM32::L0::FLASHEx
  CMSIS::STM32::L031K6
  STM32::NoSys
)

