#include "main.h"
#include "g4_iic.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"

#include "sys_tick.h"
#include "g4_iic.h"
#include "g4_gpio.h"
#include "sd_card.hpp"
#include "stm32g4xx_hal.h"

#include "scd4x.hpp"
#include "rtc_rv3028.hpp"

#include <cstdint>
#include <stdio.h>
#include <stm32g474xx.h>
#include <stm32g4xx_hal_gpio.h>
#include <stm32g4xx_hal_pwr_ex.h>
#include <string.h>

#include <stm32g4xx_hal_flash_ex.h>

#include <stm32g4xx_hal_def.h>
#include <stm32g4xx_hal_i2c.h> 
#include <stm32g4xx_hal_uart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_i2c.h>


#include <stm32g4xx_hal_flash_ex.h>
#include "fatfs.h"
#include <array>

void SystemClock_Config(void);
extern "C" int _write(int file, char *ptr, int len)
{
    // HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
};


int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* MCU POWER ON PIN PA0 */  
    gpio_init_t gpio_leds = {
            .pins = GPIO_PIN_0,
            .mode = GPIO_mode_output,
            .output_type = GPIO_otype_pushpull,
            .pull = GPIO_pupd_no,
            .speed = GPIO_speed_low,
            .alternate = 0
    };
    gpio_init(GPIOA, &gpio_leds); 
    gpio_set(GPIOA, GPIO_PIN_0);

    /* ---------------------- */

    // MX_USART2_UART_Init();
    // sys_delay(2000);
    // HAL_UART_MspDeInit(&huart2); 
    sys_delay(2000);
    HAL_PWREx_EnableLowPowerRunMode();
    sys_delay(2000);
    HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFE); 
     


    /* Pheripheral Init  */ 
    gpio_leds.pins = GPIO_PIN_3;
    gpio_init(GPIOA, &gpio_leds);
    gpio_leds.pins = GPIO_PIN_0;
    gpio_init(GPIOB, &gpio_leds);   
    // MX_USART2_UART_Init();
    /* IIC RTC and CO2 */
    gpio_init_t gpio_settings = {
        .pins = GPIO_pin_8 | GPIO_PIN_9,
        .mode = GPIO_mode_alternate,
        .output_type = GPIO_otype_opendrain,
        .pull = GPIO_pupd_no,
        .speed = GPIO_speed_veryhigh,
        .alternate = GPIO_af_4};
    gpio_init(GPIOA, &gpio_settings);
    iic_preinit(); 
    iic_init_t iic_setting = {0x10D19CE4}; 
    iic_init(I2C2, &iic_setting);
    /* ------------------------ */
    /*  --------  RTC Alarm  ---------------- */
    RTC_RV3028 rtc(I2C2);
    rtc.setAlarm(0, 30);
    uint8_t id = rtc.readID();
    printf("RTC ID: 0x%02X\n\r", id);
    /* --------------------------------------  */   

    /*  --------  co2 readout loop ---------------- */
    SCD4X co2_sensor(I2C2);
    printf("Serial number: %llu\n\r", co2_sensor.getSensorSerialNumber());
    std::array<SCD4X::Measurement, 1> measurements;
    co2_sensor.startPeriodicMeasurement();  

    for (size_t i = 0; i < measurements.size(); i++)
    {
        while (!co2_sensor.getDataReadyStatus())
        {
            sys_delay(500);
        } 
        measurements[i] = co2_sensor.getMeasurement();
        gpio_toogle(GPIOB, GPIO_PIN_0); 
    }
    /* --------------------------------------  */   


    /* ----------- Sd card -------------*/ 
    printf("\r\n~ SD card demo by kiwih ~\r\n\r\n");
    SdCard sd_card;
    sd_card.init();

    BYTE readBuf[100];
     
    for (size_t i = 0; i < measurements.size(); i++)
    {
        int len = snprintf((char*)readBuf, sizeof(readBuf), "CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\n\r",
            measurements[i].co2_ppm, measurements[i].temperature, measurements[i].humidity);
        len -= 1;
        sd_card.writeMeasurementData(readBuf, len);
        gpio_toogle(GPIOA, GPIO_PIN_3); 
    }

    /* --------------------------------------  */   

    gpio_reset(GPIOA, GPIO_PIN_0);
    while(1);
}


// void SystemClock_Config(void) 
// {
//     RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//     /** Configure the main internal regulator output voltage
//     */
//     HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

//     /** Initializes the RCC Oscillators according to the specified parameters
//     * in the RCC_OscInitTypeDef structure.
//     */
//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//     RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//     RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//     RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//     RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//     RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
//     RCC_OscInitStruct.PLL.PLLN = 12;
//     RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//     RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
//     RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//     if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//     {
//         Error_Handler();
//     }

//     /** Initializes the CPU, AHB and APB buses clocks
//     */
//     RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//     RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//     RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//     RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

//     if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
//     {
//         Error_Handler();
//     }
// }

// void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//   /** Configure the main internal regulator output voltage
//   */
//   HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV16;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
    while(1)
    {
        sys_delay(500);
        gpio_toogle(GPIOA, GPIO_PIN_3);
        gpio_toogle(GPIOB, GPIO_PIN_0);
    }
}

