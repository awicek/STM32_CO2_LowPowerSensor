#include "main.h"
#include "g4_iic.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"

#include "sys_tick.h"
#include "g4_iic.h"
#include "g4_gpio.h"
#include "stm32g4xx_hal.h"

#include "scd4x.hpp"
#include "rtc_rv3028.hpp"

#include <cstdint>
#include <stdio.h>
#include <stm32g474xx.h>
#include <stm32g4xx_hal_gpio.h>
#include <string.h>

#include <stm32g4xx_hal_flash_ex.h>

#include <stm32g4xx_hal_def.h>
#include <stm32g4xx_hal_i2c.h> 
#include <stm32g4xx_hal_uart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_i2c.h>

#include <stm32g4xx_hal_flash_ex.h>
#include "fatfs.h"

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

    MX_SPI1_Init();
    MX_USART2_UART_Init();

    gpio_leds.pins = GPIO_PIN_3;
    gpio_init(GPIOA, &gpio_leds);
    gpio_leds.pins = GPIO_PIN_0;
    gpio_init(GPIOB, &gpio_leds);    

    
    gpio_set(GPIOA, GPIO_PIN_3);
    gpio_set(GPIOB, GPIO_PIN_0);

    for (size_t i = 0; i < 4; i++)
    {
        gpio_toogle(GPIOA, GPIO_PIN_3);
        gpio_toogle(GPIOB, GPIO_PIN_0);
        sys_delay(200);
    }
    // gpio_reset(GPIOA, GPIO_PIN_0);

    // while(1)
    // {
    //     sys_delay(500);
    //     gpio_toogle(GPIOA, GPIO_PIN_3);
    //     gpio_toogle(GPIOB, GPIO_PIN_0);
    // }
   
    
    /* IIC  RTC pins PA8 PA9  */
    gpio_init_t gpio_settings = {
        .pins = GPIO_pin_8 | GPIO_PIN_9,
        .mode = GPIO_mode_alternate,
        .output_type = GPIO_otype_opendrain,
        .pull = GPIO_pupd_no,
        .speed = GPIO_speed_veryhigh,
        .alternate = GPIO_af_4};
        gpio_init(GPIOA, &gpio_settings);

    // /* IIC CO2 GPIO PA15 PB7 */
    // gpio_settings.pins = GPIO_PIN_15;
    // gpio_settings.mode = GPIO_mode_output;
    // gpio_init(GPIOA, &gpio_settings);
    // gpio_settings.pins = GPIO_PIN_7;
    // gpio_init(GPIOB, &gpio_settings);
    // gpio_set(GPIOA, GPIO_PIN_15);
    // gpio_set(GPIOB, GPIO_PIN_7);
    // sys_delay(100);
    
    // gpio_settings.mode = GPIO_mode_alternate;
    // gpio_init(GPIOB, &gpio_settings);
    // gpio_settings.pins = GPIO_PIN_15;
    // gpio_init(GPIOA, &gpio_settings);
            
        
        
        
    iic_preinit(); 
    iic_init_t iic_setting = {0x10D19CE4}; 
    iic_init(I2C2, &iic_setting);
    // iic_init(I2C1, &iic_setting); 

    /*  --------  co2 test  ---------------- */
    printf("ahoj %d :-D \n\r", 1);
    SCD4X co2_sensor(I2C2);
    sys_delay(1000);
    printf("Serial number: %llu\n\r", co2_sensor.getSensorSerialNumber());

    co2_sensor.startPeriodicMeasurement(); 
    
    while (!co2_sensor.getDataReadyStatus())
    {
        sys_delay(1000);
    } 
    auto measurement = co2_sensor.getMeasurement();
    printf("CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\n\r",
                measurement.co2_ppm, measurement.temperature, measurement.humidity);

    /* --------------------------------------  */   

    /*  --------  rtc test  ---------------- */
    RTC_RV3028 rtc(I2C2);
    rtc.setAlarm(0, 1);
    uint8_t id = rtc.readID();
    printf("RTC ID: 0x%02X\n\r", id);
    
    for (size_t i = 0; i < 4; i++)
    {
        gpio_toogle(GPIOA, GPIO_PIN_3); 
        gpio_toogle(GPIOB, GPIO_PIN_0);
        sys_delay(100);
    }
    /* --------------------------------------  */   
    

    gpio_reset(GPIOA, GPIO_PIN_0);
    while(1);

    /* ----------- Sd card -------------*/ 
    MX_FATFS_Init();
    printf("\r\n~ SD card demo by kiwih ~\r\n\r\n");

    HAL_Delay(1000); //a short delay is important to let the SD card settle

    FATFS FatFs; 	//Fatfs handle
    FIL fil; 		//File handle
    FRESULT fres; //Result after operations

    //Open the file system
    while ((fres = f_mount(&FatFs, "", 1)) != FR_OK)
    {
        printf("f_mount error (%i)\r\n", fres);
        HAL_Delay(1000);
    }

    //Let's get some statistics from the SD card
    DWORD free_clusters, free_sectors, total_sectors;

    FATFS* getFreeFs;

    fres = f_getfree("", &free_clusters, &getFreeFs);
    if (fres != FR_OK) {
        printf("f_getfree error (%i)\r\n", fres);
        while(1);
    }

    //Formula comes from ChaN's documentation
    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors = free_clusters * getFreeFs->csize;

    printf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

    //Now let's try to open file "test.txt"
    fres = f_open(&fil, "test.txt", FA_READ);
    if (fres != FR_OK) {
        printf("f_open error (%i)\r\n", fres);
    }
    printf("I was able to open 'test.txt' for reading!\r\n");

    //Read 30 bytes from "test.txt" on the SD card
    BYTE readBuf[100];

    //We can either use f_read OR f_gets to get data out of files
    //f_gets is a wrapper on f_read that does some string formatting for us
    TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
    if(rres != 0) {
        printf("Read string from 'test.txt' contents: %s\r\n", readBuf);
    } else {
        printf("f_gets error (%i)\r\n", fres);
    }

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);

    //Now let's try and write a file "write.txt"
    // fres = f_open(&fil, "co2_data.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
    fres = f_open(&fil, "co2_data.txt", FA_WRITE | FA_OPEN_APPEND);
    if(fres == FR_OK) {
        printf("I was able to open 'co2_data.txt' for writing\r\n");
    } else {
        printf("f_open error (%i)\r\n", fres);
    }

    //Copy in a string

    
    
    /* ------------------------------------------------- */

    printf("ahoj %d :-D \n\r", 1);
        
    /* CO2 readout loop  */ 
    sys_delay(1000);
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
    
    gpio_reset(GPIOA, GPIO_PIN_3);
    gpio_reset(GPIOB, GPIO_PIN_0);    

    co2_sensor.startPeriodicMeasurement();
    
    
    
    for (size_t i = 0; i < 200; i++)
    {
        while (!co2_sensor.getDataReadyStatus())
        {
            sys_delay(1000);
        } 
        auto measurement = co2_sensor.getMeasurement();
        printf("CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\n\r",
                measurement.co2_ppm, measurement.temperature, measurement.humidity);
       
        
        int len = snprintf((char*)readBuf, sizeof(readBuf), "CO2: %.0f ppm, T: %.2f C, RH: %.2f %%\n\r",
                measurement.co2_ppm, measurement.temperature, measurement.humidity);
        len -= 1;
        UINT bytesWrote;
        fres = f_write(&fil, readBuf, len, &bytesWrote);
        if(fres == FR_OK) {
            printf("Wrote %i bytes to 'co2_data.txt'!\r\n", bytesWrote);
        } else {
            printf("f_write error (%i)\r\n", fres);
        }
        
        gpio_toogle(GPIOA, GPIO_PIN_3);
    }
     
    gpio_reset(GPIOA, GPIO_PIN_3);
    gpio_set(GPIOB, GPIO_PIN_0);
    f_close(&fil);
    f_mount(NULL, "", -1);
    gpio_set(GPIOA, GPIO_PIN_3);

    while(1);
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
    RCC_OscInitStruct.PLL.PLLN = 12;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
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
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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

    // __disable_irq();
    while (1)
    {
    }
}

