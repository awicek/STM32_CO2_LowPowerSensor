#include "sd_card.hpp"


#include "spi.h"
#include "stm32g4xx_hal_spi.h"

SdCard::SdCard()
{
}


void SdCard::init()
{
    MX_SPI1_Init();
    MX_FATFS_Init();
    int ret = mountSdCard();
    if (ret != FR_OK)
        return;
    ret = openFile(&co2_data_file_, CO2_DATA_FILE_PATH);
    if (ret != FR_OK)
        return;
    is_initialized_ = true;
}


void SdCard::deinit()
{
    f_close(&co2_data_file_);
    f_mount(nullptr, "", 1);
    HAL_SPI_MspDeInit(&hspi1);
    is_initialized_ = false;
};


int SdCard::writeMeasurementData(const void* data, unsigned int data_size)
{
    if (!is_initialized_)
        return -1;

    UINT bytes_written;
    int ret = f_write(&co2_data_file_, data, data_size, &bytes_written);
    
    return ret;
}


int SdCard::mountSdCard()
{
    int ret = f_mount(&fat_fs_, "", 1);

    return ret;
}

int SdCard::getSdCardStats()
{
    int ret = 0;
    DWORD free_clusters, free_sectors, total_sectors; 
    FATFS* getFreeFs;

    ret = f_getfree("", &free_clusters, &getFreeFs);
    if (ret != FR_OK) 
        return ret;

    //Formula comes from ChaN's documentation
    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors = free_clusters * getFreeFs->csize;

    return ret;
}


int SdCard::openFile(FIL* file, const char* file_path)
{
    int ret = f_open(file, file_path, FA_WRITE | FA_OPEN_APPEND | FA_CREATE_ALWAYS);
    return ret;
}