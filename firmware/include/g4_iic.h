#ifndef G4_IIC_H_
#define G4_IIC_H_

#include "stm32g474xx.h"

#include "stm32g4xx_ll_bus.h"
#include <cmsis_gcc.h>
#include <stm32g4xx.h>
#include "main.h"

#ifdef __cplusplus
extern  "C" {
#endif

/**
  * @brief Set target 7-bit address 
  */ 
__STATIC_INLINE void iic_set_target_7bit_addr(I2C_TypeDef *iic, uint8_t addr)
{
    CLEAR_BIT(iic->CR2, I2C_CR2_ADD10);
    MODIFY_REG(iic->CR2, I2C_CR2_SADD_Msk, (uint32_t)addr << 1U);
}


/**
  * @brief Sets Automatic end. 
  * For Controller transmision it automaticaly generates STOP condition after NBYTES transfered.
  * For Controller reception it automaticaly generates NACK after NBYTES transfered.
  */
__STATIC_INLINE void iic_set_autoend(I2C_TypeDef *iic)
{
    SET_BIT(iic->CR2, I2C_CR2_AUTOEND);
}


/**
  * @brief Start transaction.
  */
__STATIC_INLINE void iic_start_transaction(I2C_TypeDef *iic)
{
    SET_BIT(iic->CR2, I2C_CR2_START);
}


/**
  * @brief Setup Controler Read transaction. 
  * @param size Expected number of bytes to receive.
  */
__STATIC_INLINE void iic_setup_controler_read_transaction(I2C_TypeDef *iic, uint8_t size)
{
    SET_BIT(iic->CR2, I2C_CR2_RD_WRN);
    MODIFY_REG(iic->CR2, I2C_CR2_NBYTES_Msk, (uint32_t)size << I2C_CR2_NBYTES_Pos);
}

/**
  * @brief Setup Controler Write transaction.
  * @param size Number of bytes to send.
  */
__STATIC_INLINE void iic_setup_controler_write_transaction(I2C_TypeDef *iic, uint8_t size)
{
    CLEAR_BIT(iic->CR2, I2C_CR2_RD_WRN);
    MODIFY_REG(iic->CR2, I2C_CR2_NBYTES_Msk, (uint32_t)size << I2C_CR2_NBYTES_Pos);
}

/* IIC init structure */
typedef struct 
{
    uint32_t timing;            /* IIC timing */
} iic_init_t;



void  iic_preinit();

/**
 *  @brief Initilalize the iic peripheral with given settings.
 *  Befor calling this function, the clock has to be enbled for the corresponding iic peripheral.
 *  Also the GPIO pins for SCL and SDA should be configured before calling this funciton.
 *  @param iic IIC peripheral to initialize.
 */
void iic_init(I2C_TypeDef *iic, iic_init_t *settings);

/**
 *  @brief Transmit data to slave device.
 *  Works only in the controler/master mode.
 *  @param iic IIC peripheral to use for transmission.
 *  @param addr 7-bit address of the slave device. It will be shifted left by this function. 
 *  @param data Pointer to the data to transmit.
 *  @param size Number of bytes to transmit. 
 *  
 *  @return Number of transmitted bytes.
 */
uint8_t iic_transmit(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint8_t size);

/**
 *  @brief Receive data from slave device.
 *  Works only in the controler/master mode.
 *  @param iic IIC peripheral to use for reception.
 *  @param addr 7-bit address of the slave device. It will be shifted left by this function. 
 *  @param data Pointer to the buffer to store received data.
 *  @param size Number of bytes to receive.
 * 
 *  @return Number of received bytes.   
 */
uint8_t iic_receive(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif 
#endif // G4_IIC_H_
