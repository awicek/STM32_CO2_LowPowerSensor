#ifndef G4_GPIO_H_
#define G4_GPIO_H_

#include "stm32g474xx.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/* Returns the position of the pin. pin 0b1000 -> returns 3 */
#define get_pin_pos(pin) (__CLZ(__RBIT(pin))) 

 
/* GPIO init structure */
typedef struct
{
    uint32_t pins;           /* Which GPIO pin */ 
    uint8_t mode;
    uint8_t output_type;
    uint32_t pull;          /* Pull-up or Pull-down activation */
    uint32_t speed;         /* Speed for the selected pins */ 
    uint32_t alternate;     /* Activates alternate function of the pin */ 
} gpio_init_t;



/* GPIO MODE */
#define GPIO_mode_input           0x0
#define GPIO_mode_output          0x1
#define GPIO_mode_alternate       0x2
#define GPIO_mode_analog          0x3
#define GPIO_mode_mask            0x3

/* GPIO OTYPE  */
#define GPIO_otype_pushpull       0x0
#define GPIO_otype_opendrain      0x1
#define GPIO_otype_mask           0x1

/* GPIO OSPEED */
#define GPIO_speed_low            0x0
#define GPIO_speed_medium         0x1
#define GPIO_speed_high           0x2
#define GPIO_speed_veryhigh       0x3
#define GPIO_speed_mask           0x3

/* GPIO PUPD */
#define GPIO_pupd_no              0x0
#define GPIO_pupd_pullup          0x1
#define GPIO_pupd_pulldown        0x2
#define GPIO_pupd_mask            0x3

/* GPIO pin mask */
#define GPIO_pin_0               ((uint16_t)0x0001)
#define GPIO_pin_1               ((uint16_t)0x0002)
#define GPIO_pin_2               ((uint16_t)0x0004)
#define GPIO_pin_3               ((uint16_t)0x0008)
#define GPIO_pin_4               ((uint16_t)0x0010)
#define GPIO_pin_5               ((uint16_t)0x0020)
#define GPIO_pin_6               ((uint16_t)0x0040)
#define GPIO_pin_7               ((uint16_t)0x0080)
#define GPIO_pin_8               ((uint16_t)0x0100)
#define GPIO_pin_9               ((uint16_t)0x0200)
#define GPIO_pin_10              ((uint16_t)0x0400)
#define GPIO_pin_11              ((uint16_t)0x0800)
#define GPIO_pin_12              ((uint16_t)0x1000)
#define GPIO_pin_13              ((uint16_t)0x2000)
#define GPIO_pin_14              ((uint16_t)0x4000)
#define GPIO_pin_15              ((uint16_t)0x8000)
#define GPIO_pin_mask            ((uint32_t)0xFFFF)  /* Mask for all the pins */

/* GPIO Aternate Functions */
#define GPIO_af_mask             0xFU
#define GPIO_af_0                0x0
#define GPIO_af_1                0x1
#define GPIO_af_2                0x2
#define GPIO_af_3                0x3
#define GPIO_af_4                0x4
#define GPIO_af_5                0x5
#define GPIO_af_6                0x6
#define GPIO_af_7                0x7
#define GPIO_af_8                0x8
#define GPIO_af_9                0x9
#define GPIO_af_10               0xA
#define GPIO_af_11               0xB
#define GPIO_af_12               0xC
#define GPIO_af_13               0xD
#define GPIO_af_14               0xE
#define GPIO_af_15               0xF


/**
 *  Initialize gpio pins.
 *  @param port GPIO port (GPIOA, GPIOB, GPIOC, GPIOD)
 *  @param settings Pointer to gpio_init_t structure with settings for the pins.
 *  Is it possible to set multiple pins at once. By setting parameter
 *  settings->pins to GPIO_pin_0 | GPIO_pin_1 for example.
 */
void gpio_init(GPIO_TypeDef *port, gpio_init_t *parameters); 

/**
 *  Read gpio pins.
 *  @return 1 if the pin is high, 0 if the pin is low.
 */
int gpio_read(GPIO_TypeDef *port, uint16_t pin);

/** 
 *   Set gpio pin. Can set multiple pins at once by ORing them together.
 */
void gpio_set(GPIO_TypeDef *port, uint16_t pin);

/** 
 *   Reset gpio pin. Can reset multiple pins at once by ORing them together.
 */
void gpio_reset(GPIO_TypeDef *port, uint16_t pin);

/**
 *  Toggle gpio pin. Can toggle !!! only one !!! at once.
 */
void gpio_toogle(GPIO_TypeDef *port, uint16_t pin);




#ifdef __cplusplus
}
#endif

#endif // G4_GPIO_H_
