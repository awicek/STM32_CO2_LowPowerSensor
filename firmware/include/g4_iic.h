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



void iic_preinit()
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
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
}

/* IIC init structure */
typedef struct 
{
    uint32_t timing;            /* IIC timing */
} iic_init_t;


/* IIC TIMINGR */
#define IIC_timingr_mask        0xF0FFFFFFU

/* IIC_CR2 Control Register 2 */
#define IIC_cr2_rdwr_write              0x0U
#define IIC_cr2_rdwr_read               0x1U
#define IIC_cr2_add10_10bit             0x1U
#define IIC_cr2_add10_7bit              0x0U



typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t sadd       : 10;
        uint32_t rd_wrn     : 1;
        uint32_t add10      : 1;
        uint32_t head10r    : 1;
        uint32_t start      : 1;
        uint32_t stop       : 1;
        uint32_t nack       : 1;
        uint32_t nbytes     : 8;
        uint32_t reload     : 1;
        uint32_t autoend    : 1;
        uint32_t pecbyte    : 1;
    } bit;
} IIC_cr2_r;

typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t pe         : 1;        // Peripheral enable 
        uint32_t txie       : 1;
        uint32_t rxie       : 1;
        uint32_t addrie     : 1;
        uint32_t nackie     : 1;
        uint32_t stopie     : 1; 
        uint32_t rcie       : 1;
        uint32_t errie      : 1;
        uint32_t dnf        : 4;
        uint32_t anfoff     : 1;
        uint32_t reserved1  : 1;
        uint32_t txdmaen    : 1;
        uint32_t rxdmaen    : 1;
        uint32_t sbc        : 1;
        uint32_t nostretch  : 1;
        uint32_t wupen      : 1; 
        uint32_t gcen       : 1;
        uint32_t smbhen     : 1;
        uint32_t smdden     : 1;
        uint32_t alerten    : 1;
        uint32_t pecen      : 1;
    } bit;
} IIC_cr1_r;


void iic_init(I2C_TypeDef *iic, iic_init_t *settings)
{
    /* Disable IIC */
    CLEAR_BIT(iic->CR1, I2C_CR1_PE);

    /* Set timing register */ 
    iic->TIMINGR = (iic->TIMINGR & IIC_timingr_mask) | settings->timing;

    /* Set analog and digital filters */

    /* Set control register 1 and 2 */
    SET_BIT(iic->CR1, I2C_CR1_PE);

    
}

void iic_set_timing(I2C_TypeDef *iic, uint32_t timing)
{
    iic->TIMINGR = (iic->TIMINGR & IIC_timingr_mask) | timing;
}


#ifdef __cplusplus
}
#endif 
#endif // G4_IIC_H_
