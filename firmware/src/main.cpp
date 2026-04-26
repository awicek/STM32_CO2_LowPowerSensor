#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"


#include "stm32g4xx_hal.h"

#include <cstdint>
#include <stdio.h>
#include <string.h>

#include <stm32g4xx_hal_flash_ex.h>

#include <stm32g4xx_hal_def.h>
#include <stm32g4xx_hal_i2c.h>
#include <stm32g4xx_hal_uart.h>

void SystemClock_Config(void);

typedef struct __attribute__((packed)) 
{
    uint32_t seraial_number_1;
    uint32_t seraial_number_2;
    uint8_t crc;
} co2_serial_number_t;

class SCD40 
{
public:
    constexpr static uint8_t ADDRESS = 0x62 << 1;

    // ===== Basic commands =====
    constexpr static uint16_t START_PERIODIC_MEASUREMENT      = 0xB121;
    constexpr static uint16_t READ_MEASUREMENT                = 0x05EC;
    constexpr static uint16_t STOP_PERIODIC_MEASUREMENT       = 0x863F;

    // ===== Temperature / altitude =====
    constexpr static uint16_t SET_TEMPERATURE_OFFSET          = 0x1D24;
    constexpr static uint16_t GET_TEMPERATURE_OFFSET          = 0x1823;
    constexpr static uint16_t SET_SENSOR_ALTITUDE             = 0x2724;
    constexpr static uint16_t GET_SENSOR_ALTITUDE             = 0x2223;

    // ===== Pressure =====
    constexpr static uint16_t SET_AMBIENT_PRESSURE            = 0x00E0;
    constexpr static uint16_t GET_AMBIENT_PRESSURE            = 0x00E0;

    // ===== Calibration =====
    constexpr static uint16_t PERFORM_FORCED_RECALIBRATION    = 0x2F36;
    constexpr static uint16_t SET_ASC_ENABLED                 = 0x1624;
    constexpr static uint16_t GET_ASC_ENABLED                 = 0x1323;

    // ===== Low power =====
    constexpr static uint16_t START_LOW_POWER_PERIODIC_MEAS   = 0xAC21;
    constexpr static uint16_t GET_DATA_READY_STATUS           = 0xB8E4;

    // ===== Advanced =====
    constexpr static uint16_t PERSIST_SETTINGS                = 0x1536;
    constexpr static uint16_t GET_SERIAL_NUMBER               = 0x8236;
    constexpr static uint16_t PERFORM_SELF_TEST               = 0x3936;
    constexpr static uint16_t FACTORY_RESET                   = 0x3236;
    constexpr static uint16_t REINIT                          = 0x4636;
    constexpr static uint16_t GET_SENSOR_VARIANT              = 0x2F20;

    // ===== Single shot (SCD41) =====
    constexpr static uint16_t MEASURE_SINGLE_SHOT             = 0x9D21;
    constexpr static uint16_t MEASURE_SINGLE_SHOT_RHT_ONLY    = 0x9621;
    constexpr static uint16_t POWER_DOWN                      = 0xE036;
    constexpr static uint16_t WAKE_UP                         = 0xF636;

    // ===== ASC periods =====
    constexpr static uint16_t SET_ASC_INITIAL_PERIOD          = 0x4524;
    constexpr static uint16_t GET_ASC_INITIAL_PERIOD          = 0x4023;
    constexpr static uint16_t SET_ASC_STANDARD_PERIOD         = 0x4E24;
    constexpr static uint16_t GET_ASC_STANDARD_PERIOD         = 0x4B23;
};



int main(void)
{
    HAL_Init();
    char char_buff[50];
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    MX_SPI2_Init();

    uint8_t buffer[20];
    HAL_Delay(1000);
    
    while (HAL_ERROR == HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS, 
            (uint8_t*)&SCD40::START_PERIODIC_MEASUREMENT,
            2, HAL_MAX_DELAY))
    {
        HAL_Delay(8000);
        break;
    }
    
    HAL_Delay(1000);
    
    while (1)
    {
        
        // HAL_Delay(500);
        // HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS, (uint8_t*)&SCD40::GET_SENSOR_VARIANT, 2, HAL_MAX_DELAY);
        // HAL_Delay(10);
        // HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS, buffer, 3, HAL_MAX_DELAY);
        // if (buffer[2] == 0b00010000)
        // {
        //     HAL_UART_Transmit(&huart2, (uint8_t*)"SCD40\n\r", 7, HAL_MAX_DELAY);
        // }
        // else
        // {
        //     HAL_UART_Transmit(&huart2, (uint8_t*)"SCD41\n\r", 7, HAL_MAX_DELAY);
        // }

        // HAL_Delay(100);

        // HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS,
        //         (uint8_t*)&SCD40::GET_SENSOR_ALTITUDE, 2, HAL_MAX_DELAY);
        // HAL_Delay(10);
        // HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS,
        //     buffer, 3, HAL_MAX_DELAY);
        while (true)
        {
            
            HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS,
                (uint8_t*)&SCD40::GET_DATA_READY_STATUS, 2, HAL_MAX_DELAY);
                HAL_Delay(10);
                HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS,
                    buffer, 3, HAL_MAX_DELAY);
            if ((buffer[0] & 0x07) || (buffer[1] & 0xff))
            {
                break;
            }
            HAL_Delay(100);
        } 

        HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS,
                (uint8_t*)&SCD40::READ_MEASUREMENT, 2, HAL_MAX_DELAY);
        HAL_Delay(10);
        HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS,
            buffer, 9, HAL_MAX_DELAY);


        uint16_t co2_raw  = (buffer[0] << 8) | buffer[1];
        uint16_t temp_raw = (buffer[3] << 8) | buffer[4];
        uint16_t rh_raw   = (buffer[6] << 8) | buffer[7];
        float temperature = -45.0f + 175.0f * ((float)temp_raw / 65535.0f);
        float humidity    = 100.0f * ((float)rh_raw / 65535.0f);
        float co2         = (float)co2_raw;

        char out[64];

        sprintf(out, "CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\r\n",
                co2, temperature, humidity);


        HAL_UART_Transmit(&huart2, (uint8_t*)out, strlen(out), HAL_MAX_DELAY);
        
        HAL_Delay(4000);
    }
}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler();
    }
}


void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

