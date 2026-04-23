#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"

#include "fatfs.h"

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"

#include <cstdint>
#include <stm32l0xx_hal_flash_ex.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <stm32l0xx_hal_def.h>
#include <stm32l0xx_hal_i2c.h>
#include <stm32l0xx_hal_uart.h>

void SystemClock_Config(void);

void myprintf(const char *fmt, ...)
{
    static char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    int len = strlen(buffer);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
}

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
    // MX_I2C1_Init();
    MX_SPI1_Init();
    MX_FATFS_Init();


    // HAL_FLASHEx_DATAEEPROM_Unlock(); 
    // HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080000 ,  0x33);
    // HAL_FLASHEx_DATAEEPROM_Lock();

    // while(1){};
   
    
    // myprintf("\r\n~ SD card demo by kiwih ~\r\n\r\n");

    HAL_Delay(1000); //a short delay is important to let the SD card settle

    //some variables for FatFs
    FATFS FatFs; 	//Fatfs handle
    FIL fil; 		//File handle
    FRESULT fres; //Result after operations

    //Open the file system
    fres = f_mount(&FatFs, "", 1); //1=mount now
    if (fres != FR_OK) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"f_mount error\r\n", 15, HAL_MAX_DELAY);
  	while(1);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"mounted", 9, HAL_MAX_DELAY);
    //Let's get some statistics from the SD card
    DWORD free_clusters, free_sectors, total_sectors;

    FATFS* getFreeFs;

    fres = f_getfree("", &free_clusters, &getFreeFs);
    if (fres != FR_OK) {
  	// myprintf("f_getfree error (%i)\r\n", fres);
  	while(1);
    }

    //Formula comes from ChaN's documentation
    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors = free_clusters * getFreeFs->csize;

    // myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

    //Now let's try to open file "test.txt"
    fres = f_open(&fil, "test.txt", FA_READ);
    if (fres != FR_OK) {
  	// myprintf("f_open error (%i)\r\n", fres);
  	while(1);
    }
    // myprintf("I was able to open 'test.txt' for reading!\r\n");

    //Read 30 bytes from "test.txt" on the SD card
    BYTE readBuf[30];

    //We can either use f_read OR f_gets to get data out of files
    //f_gets is a wrapper on f_read that does some string formatting for us
    TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
    if(rres != 0) {
  	// myprintf("Read string from 'test.txt' contents: %s\r\n", readBuf);
    } else {
  	// myprintf("f_gets error (%i)\r\n", fres);
    }

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);

    //Now let's try and write a file "write.txt"
    fres = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
    if(fres == FR_OK) {
  	// myprintf("I was able to open 'write.txt' for writing\r\n");
    } else {
  	// myprintf("f_open error (%i)\r\n", fres);
    }

    //Copy in a string
    strncpy((char*)readBuf, "a new file is made!", 19);
    UINT bytesWrote;
    fres = f_write(&fil, readBuf, 19, &bytesWrote);
    if(fres == FR_OK) {
  	// myprintf("Wrote %i bytes to 'write.txt'!\r\n", bytesWrote);
    } else {
  	// myprintf("f_write error (%i)\r\n", fres);
    }

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);

    //We're done, so de-mount the drive
    f_mount(NULL, "", 0);

    while (1)
    {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);         
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

