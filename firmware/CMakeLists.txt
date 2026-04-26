cmake_minimum_required(VERSION 3.13)

# Generates compile_commands.json located in ../build/compile_commands.json
# It is used by the clangd or cpp extensions in VSCode for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set path to the STM32Cube libraries and toolchain for compilation
set(STM32_TOOLCHAIN_PATH /opt/st/stm32cubeclt_1.18.0/GNU-tools-for-STM32)
set(STM32_CUBE_G4_PATH /home/richard/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.2)
set(STM32_TARGET_TRIPLET arm-none-eabi)

# Path to the cmake used to find all the STM32Cube libraries 
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

# Enables C CPP and ASM 
project(example_interface C ASM CXX)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall)
add_definitions(-Wextra)

find_package(CMSIS COMPONENTS STM32G4 REQUIRED)
find_package(HAL COMPONENTS STM32G4 RCC PWR PCD UART USART GPIO DMA CORTEX SPI TIM I2C FLASH)
add_definitions(-DUSE_FULL_LL_DRIVER)

add_executable(main
    src/main.cpp

    src/gpio.c
    src/usart.c
    src/i2c.c 
    src/syscalls.c 
    src/spi.c

    src/stm32g4xx_hal_msp.c   
    src/stm32g4xx_it.c
    
    fatfs/app/fatfs.c
    
    fatfs/core/diskio.c
    fatfs/core/ff.c 
    fatfs/core/ff_gen_drv.c
    fatfs/core/option/syscall.c

    fatfs/target/user_diskio.c
    fatfs/target/user_diskio_spi.c
)

target_include_directories(main PUBLIC
    include
    fatfs/app
    fatfs/core
    fatfs/target
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(main PRIVATE -Og -g3 -fdata-sections -ffunction-sections -fno-inline)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(main PRIVATE -O2)
endif()

# prints the memory usage at the end of the linking process 
target_link_options(main PUBLIC -Wl,--print-memory-usage,--undefined=uxTopUsedPriority)

target_link_libraries(main
  HAL::STM32::G4::RCC
  HAL::STM32::G4::RCCEx
  HAL::STM32::G4::PWR
  HAL::STM32::G4::PWREx
  HAL::STM32::G4::DMA
  HAL::STM32::G4::GPIO
  HAL::STM32::G4::CORTEX
  HAL::STM32::G4::UART
  HAL::STM32::G4::UARTEx
  HAL::STM32::G4::USART
  HAL::STM32::G4::USARTEx
  HAL::STM32::G4::SPI
  HAL::STM32::G4::TIM
  HAL::STM32::G4::TIMEx
  HAL::STM32::G4::I2C
  HAL::STM32::G4::I2CEx
  HAL::STM32::G4::FLASH
  HAL::STM32::G4::FLASHEx
  CMSIS::STM32::G474RE
  STM32::NoSys
)

