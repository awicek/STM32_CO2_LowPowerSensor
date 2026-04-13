#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#include <cstdint>
#include <stm32l0xx_hal_def.h>
#include <stm32l0xx_hal_i2c.h>
#include <stm32l0xx_hal_uart.h>

void SystemClock_Config(void);

typedef struct __attribute__((packed)) 
{
    uint32_t seraial_number_1;
    uint32_t seraial_number_2;
    uint8_t crc;
} co2_serial_number_t;

int main(void)
{
    HAL_Init();
    
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    
    uint16_t co2_sensor_address = 0x62 << 1;
    uint16_t co2_cmd_get_serrial_number =  0x8236;
    co2_serial_number_t co2_data;
    
    HAL_Delay(100);

    while(HAL_ERROR == HAL_I2C_Master_Transmit(&hi2c1, co2_sensor_address,
            (uint8_t*)&co2_cmd_get_serrial_number, 2, 10)){

        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);         
            };

    HAL_Delay(10); 

    HAL_I2C_Master_Receive(&hi2c1, co2_sensor_address ,
            (uint8_t*)&co2_data, sizeof(co2_data), HAL_MAX_DELAY);
   

    
    while (1)
    {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);         
        HAL_UART_Transmit(&huart2, (uint8_t*)&co2_data, sizeof(co2_data), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 15, HAL_MAX_DELAY);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
    RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
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

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
      Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

