#ifndef G4_SPI_H_
#define G4_SPI_H_

#include "stm32g474xx.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *  @brief SPI init structure
 */
typedef struct 
{
    uint32_t todo;
} spi_init_t; 


/**
 *  @brief Initialize SPI peripheral. 
 *  
 */
void spi_init(SPI_TypeDef)


#ifdef __cplusplus
}
#endif

#endif // G4_SPI_H_