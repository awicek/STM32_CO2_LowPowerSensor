#ifndef G4_IIC_H_
#define G4_IIC_H_

#include "stm32g474xx.h"

#ifdef __cplusplus
extern  "C" {
#endif

// closck source 
// I2CCLK
// PCLK
/*
PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
{
    Error_Handler();
}

__HAL_RCC_I2C1_CLK_ENABLE();
*/

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
    // TODO: make sure that iic is disabled before setting enything 

    /* Set timing register */ 
    iic->TIMINGR = (iic->TIMINGR & IIC_timingr_mask) | settings->timing;

    /* Set analog and digital filters */

    /* Set control register 1 and 2 */
    IIC_cr1_r temp;
    temp.reg = iic->CR1;
    temp.bit.pe = 1;
    iic->CR1 = temp.reg;
}

void iic_set_timing(I2C_TypeDef *iic, uint32_t timing)
{
    iic->TIMINGR = (iic->TIMINGR & IIC_timingr_mask) | timing;
}


#ifdef __cplusplus
}
#endif 
#endif // G4_IIC_H_
