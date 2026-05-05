#ifndef G4_IIC_H_
#define G4_IIC_H_

#include "stm32g474xx.h"

#include "stm32g4xx_ll_i2c.h"
#include "stm32g4xx_ll_gpio.h"
#include "main.h"
#include "stm32g4xx_ll_i2c.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_gpio.h"


#ifdef __cplusplus
extern  "C" {
#endif



void  iic_preinit()
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    //   PC4   ------> I2C2_SCL
    //   PA8   ------> I2C2_SDA
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
}

/* IIC init structure */
typedef struct 
{
    uint32_t timing;            /* IIC timing */
} iic_init_t;


void iic_init(I2C_TypeDef *iic, iic_init_t *settings)
{
    /* Disable IIC */
    CLEAR_BIT(iic->CR1, I2C_CR1_PE);

    /* Set timing register */ 
    iic->TIMINGR = settings->timing;

    /* Set analog and digital filters */
    CLEAR_BIT(iic->CR1, I2C_CR1_ANFOFF);
    
    /* Set control register 1 and 2 */
    SET_BIT(iic->CR2, I2C_CR2_AUTOEND);
    
    /* Eneable IIC */
    SET_BIT(iic->CR1, I2C_CR1_PE);
}


int iic_transmit(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint32_t size)
{
    CLEAR_BIT(iic->CR2, I2C_CR2_ADD10);
    MODIFY_REG(iic->CR2, I2C_CR2_SADD_Msk, (uint32_t)addr << (1U));

    CLEAR_BIT(iic->CR2, I2C_CR2_RD_WRN);
    MODIFY_REG(iic->CR2, I2C_CR2_NBYTES_Msk, size << I2C_CR2_NBYTES_Pos);

    SET_BIT(iic->CR2, I2C_CR2_START);
    for (size_t i = 0; i < size; ++i)
    {
        while (!(iic->ISR & I2C_ISR_TXIS)) {};
        iic->TXDR = data[size - 1 - i];
    }

    /* Wait transfer complete */
    while (!(iic->ISR & I2C_ISR_TC)){}

    return 0;
}

int iic_receive(I2C_TypeDef *iic, uint8_t addr, uint8_t *data, uint32_t size)
{
    

    return 0;
}


#ifdef __cplusplus
}
#endif 
#endif // G4_IIC_H_
