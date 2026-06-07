#ifndef SD_CARD_HPP_
#define SD_CARD_HPP_

#include <stdbool.h>
#include "fatfs.h"

class SdCard
{

constexpr static char CO2_DATA_FILE_PATH[] = "co2_data.txt";

public:

   /**
   *  @brief Cosntructor.
   */
   SdCard();

   /**
   *  @brief Initialize the spi interface and the fat subsystem.
      Mount the SD card.
   */
   void init();

   /**
   *  @brief Unmount the SD card, deinitialize the spi interface and the fat subsystem.
   *  When Sd card is not used. It good idea to deinit it to save power.
   */
   void deinit();

   /**
   *  @brief Write measurement data to a file.
   *  @return 0 on success, negative on failure  
   */
   int writeMeasurementData(const void* data, unsigned int data_size);

private:

   /**
    *  @return 0 on success, negative on failure
    */
   int mountSdCard(); 

   /**
    *  @brief Use to get SD card stats like free space, total space ...
    */
   int getSdCardStats();

   /**
    *  @brief Open file.
    *  @return 0 on success, negative on failure
    */
   int openFile(FIL* file, const char* path);

   bool is_initialized_ = false;
   FATFS fat_fs_;
   FIL co2_data_file_;


};

#endif // SD_CARD_HPP_