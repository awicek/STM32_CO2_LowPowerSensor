#include "g4_iic.h"

void  iic_preinit()
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);


    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
}

void iic_init(I2C_TypeDef *iic, iic_init_t *settings)
{
    /* Disable IIC */
    CLEAR_BIT(iic->CR1, I2C_CR1_PE);

    /* Set timing register */ 
    iic->TIMINGR = settings->timing;

    /* Set analog and digital filters */
    CLEAR_BIT(iic->CR1, I2C_CR1_ANFOFF);
    
    /* Set control register 1 and 2 * Eneable IIC */
    SET_BIT(iic->CR1, I2C_CR1_PE);
}

uint8_t iic_transmit(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint8_t size)
{
    iic_set_target_7bit_addr(iic, addr);
    iic_set_autoend(iic);
        
    iic_setup_controler_write_transaction(iic, size);

    iic_start_transaction(iic); 
    
    for (size_t i = 0; i < size; ++i)
    {
        while (!(iic->ISR & I2C_ISR_TXIS)) {};
        iic->TXDR = data[i];
    }

    return size;
}

uint8_t iic_receive(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint32_t size)
{
    iic_set_target_7bit_addr(iic, addr);
    iic_set_autoend(iic);

    iic_setup_controler_read_transaction(iic, size);

    iic_start_transaction(iic);

    for (uint8_t i = 0; i < size; ++i)
    {
        while (!(iic->ISR & I2C_ISR_RXNE)) {};
        data[i] = iic->RXDR;
    }

    return size;
}
