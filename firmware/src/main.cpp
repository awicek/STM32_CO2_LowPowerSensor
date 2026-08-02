#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"

#include "stm32g4xx_hal.h"
#include <Legacy/stm32_hal_legacy.h>
#include <stdio.h>
#include <string.h>
#include <cstdarg>

#include "g4_gpio.h"
#include "g4_spi.h"


#include <stm32g4xx_hal_flash_ex.h>

#include <stm32g4xx_hal_def.h>
#include <stm32g4xx_hal_i2c.h>
#include <stm32g4xx_hal_uart.h>
#include <sys/cdefs.h>

#include "fatfs.h"


void SystemClock_Config(void);


extern "C" int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
};

/**
 *  @brief Shows how to use the FatFs API to read and write files on an SD card.
 */
int main(void)
{ 
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_SPI1_Init();
    setbuf(stdout, NULL);
   
    // gpio_init_t led_init = {
    //     .pins = GPIO_pin_2,
    //     .mode = GPIO_mode_output,
    //     .output_type = GPIO_otype_pushpull,
    //     .pull = GPIO_pupd_no,
    //     .speed = GPIO_speed_high,
    //     .alternate = 0,
    // };
    // gpio_init(GPIOD, &led_init);
    // gpio_set(GPIOD, GPIO_pin_2);
    
    // spi_init_t spi_init_settings = {
    //     .baudrate_prescaler = SPI_BAUDRATEPRESCALER_256,
    //     .clock_polarity = SPI_clock_polarity_low,
    //     .clock_phase = SPI_clock_phase_1edge,
    //     .frame_format = SPI_format_msb_first,
    //     .data_size =  SPI_ds_8bit
    // };


    // spi_init(SPI3, &spi_init_settings);

    // uint8_t tx_data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // spi_enable(SPI3);
    // HAL_Delay(100);
   
    // gpio_reset(GPIOD, GPIO_pin_2);
    // spi_tx_rx(SPI3, tx_data, NULL, 10);
    // gpio_set(GPIOD, GPIO_pin_2);


    // HAL_Delay(100);
    // printf("ahoj %d %d %f \r\n", 1, 2, 0.444); 
    
    // while(1){};
    printf("ahoj%d%d  %f \r\n", 1, 2, 0.444); 
    uint32_t a = 0b100000000;
    printf("a %d \r\n", a);
    uint32_t b = __CLZ(__RBIT(a));
    printf("a %d \r\n", a);
    printf("a %d  b %d /n/r", a, b);
    HAL_Delay(__CLZ(__RBIT(a)));
    HAL_Delay(1000);
    
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
    BYTE readBuf[30];

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
    fres = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
    if(fres == FR_OK) {
        printf("I was able to open 'write.txt' for writing\r\n");
    } else {
        printf("f_open error (%i)\r\n", fres);
    }

    //Copy in a string
    strncpy((char*)readBuf, "a new file is made!", 19);
    UINT bytesWrote;
    fres = f_write(&fil, readBuf, 19, &bytesWrote);
    if(fres == FR_OK) {
        printf("Wrote %i bytes to 'write.txt'!\r\n", bytesWrote);
    } else {
        printf("f_write error (%i)\r\n", fres);
    }

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);

    //We're done, so de-mount the drive
    f_mount(NULL, "", 0);
    while (1)
    {
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
    __disable_irq();
    while (1)
    {
    }
}
