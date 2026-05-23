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

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);




    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);


}

void iic_init(I2C_TypeDef *iic, iic_init_t *settings)
{
    /* Disable IIC */
    CLEAR_BIT(iic->CR1, I2C_CR1_PE);

    /* Set timing register */ 
    iic->TIMINGR = settings->timing;
 
    /* Set analog and digital filters */
    CLEAR_BIT(iic->CR1, I2C_CR1_ANFOFF);

    /* Disable auto end*/
    iic_clear_autoend(iic);
    
    /* Set control register 1 and 2 * Eneable IIC */
    SET_BIT(iic->CR1, I2C_CR1_PE);
}

uint8_t iic_transmit(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint8_t size)
{
    iic_set_target_7bit_addr(iic, addr);
    iic_setup_controler_write_transaction(iic, size);

    iic_start_transaction(iic); 

    
    for (uint8_t i = 0; i < size; ++i)
    {
        while (1)
        {
            if (iic_is_active_flag_txis(iic))
            {
                break;
            }
            if (iic_is_active_flag_nackf(iic))
            {
                iic_clear_flag_nackf(iic);
                return i;
            } 
        }
        iic->TXDR = data[i];
    }

    while(1)
    { 
        if (iic_is_active_flag_tc(iic))
        {
            iic_send_stop(iic);
            break;
        };
        if (iic_is_active_flag_nackf(iic))
        {
            iic_clear_flag_nackf(iic);
            return size - 1;
        }
    }
    return size;
}

uint8_t iic_receive(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint32_t size)
{
    iic_set_target_7bit_addr(iic, addr);

    iic_setup_controler_read_transaction(iic, size);
   
    iic_flush_rxdr(iic);

    iic_start_transaction(iic);

    for (uint8_t i = 0; i < size; ++i)
    {
        while (1)
        {
            if (iic_is_active_flag_rxne(iic))
            {
                break;
            }
            if (iic_is_active_flag_nackf(iic))
            {
                iic_clear_flag_nackf(iic);
                return i;
            } 
        }
        while (!(iic->ISR & I2C_ISR_RXNE)) {};
        data[i] = iic->RXDR;
    }

    while(!iic_is_active_flag_tc(iic)) {};
    iic_send_stop(iic);
    return size;
}
