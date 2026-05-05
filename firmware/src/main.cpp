#include "main.h"
#include "g4_iic.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"

#include "sys_tick.h"
#include "g4_iic.h"
#include "g4_gpio.h"
#include "stm32g4xx_hal.h"

#include  "scd4x.hpp"

#include <cstdint>
#include <stdio.h>
#include <string.h>

#include <stm32g4xx_hal_flash_ex.h>

#include <stm32g4xx_hal_def.h>
#include <stm32g4xx_hal_i2c.h> 
#include <stm32g4xx_hal_uart.h>

void SystemClock_Config(void);
extern "C" int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
};



int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();

    //   PC4   ------> I2C2_SCL
    gpio_init_t gpio_settings = {0};
    gpio_settings.mode = GPIO_mode_alternate;
    gpio_settings.pins = GPIO_pin_4;
    gpio_settings.output_type = GPIO_otype_opendrain;
    gpio_settings.speed = GPIO_speed_low;
    gpio_settings.pull = GPIO_pupd_pullup;
    gpio_settings.alternate = GPIO_af_4;
    gpio_init(GPIOC, &gpio_settings);
    gpio_settings.pins = GPIO_pin_8;
    gpio_init(GPIOA, &gpio_settings);

    iic_preinit(); 
    iic_init_t iic2_setting = {0x10D19CE4}; 
    iic_init(I2C2, &iic2_setting);

    SCD4X co2_sensor(I2C2); 
    uint64_t serial_number = co2_sensor.getSensorSerialNumber();
    printf("Serial number: %llu\n\r", serial_number);
    sys_delay(10);
    auto type = co2_sensor.getSensorVariant();
    if (type == SCD4X::SensorVariant::SCD40)
    {
        printf("Sensor variant: SCD40\n\r");
    }
    else if (type == SCD4X::SensorVariant::SCD41)
    {
        printf("Sensor variant: SCD41\n\r");
    }   
     
    co2_sensor.startPeriodicMeasurement();
    while (1)
    {
        sys_delay(8000);
        auto measurement = co2_sensor.getMeasurement();
        printf("CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\n\r",
                measurement.co2_ppm, measurement.temperature, measurement.humidity);
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
        // while (true)
        // {
            
        //     HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS,
        //         (uint8_t*)&SCD40::GET_DATA_READY_STATUS, 2, HAL_MAX_DELAY);
        //         HAL_Delay(10);
        //         HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS,
        //             buffer, 3, HAL_MAX_DELAY);
        //     if ((buffer[0] & 0x07) || (buffer[1] & 0xff))
        //     {
        //         break;
        //     }
        //     HAL_Delay(100);
        // } 

        // HAL_I2C_Master_Transmit(&hi2c1, SCD40::ADDRESS,
        //         (uint8_t*)&SCD40::READ_MEASUREMENT, 2, HAL_MAX_DELAY);
        // HAL_Delay(10);
        // HAL_I2C_Master_Receive(&hi2c1, SCD40::ADDRESS,
        //     buffer, 9, HAL_MAX_DELAY);


        // uint16_t co2_raw  = (buffer[0] << 8) | buffer[1];
        // uint16_t temp_raw = (buffer[3] << 8) | buffer[4];
        // uint16_t rh_raw   = (buffer[6] << 8) | buffer[7];
        // float temperature = -45.0f + 175.0f * ((float)temp_raw / 65535.0f);
        // float humidity    = 100.0f * ((float)rh_raw / 65535.0f);
        // float co2         = (float)co2_raw;

        // char out[64];

        // printf("CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\r\n",
        //         co2, temperature, humidity);


        // HAL_UART_Transmit(&huart2, (uint8_t*)out, strlen(out), HAL_MAX_DELAY);
        
        // HAL_Delay(4000);
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

