#include "g4_gpio.h"


void gpio_init(GPIO_TypeDef *port, gpio_init_t *settings)
{
    uint32_t temp;
    uint32_t position; 

    /* Asserts */
    settings->pins &= GPIO_pin_mask;

    while ((settings->pins) != 0U)
    {
        position = get_pin_pos(settings->pins);
        settings->pins &= ~(1U << position);
        
        /* 1. Set gpio mode  */
        temp = port->MODER;
        temp &= ~(GPIO_mode_mask << (position * 2U));   
        temp |= ((settings->mode & GPIO_mode_mask) << (position * 2U));
        port->MODER = temp;

        /* 2. Set output type and speed */
        if ((settings->mode & GPIO_mode_mask) == GPIO_mode_input || 
            (settings->mode & GPIO_mode_mask) == GPIO_mode_alternate)
        {
            temp = port->OTYPER;
            temp &= ~(GPIO_otype_mask << position);
            temp |= (((settings->output_type) & GPIO_otype_mask) << position);
            port->OTYPER = temp;

            temp = port->OSPEEDR;
            temp &= ~(GPIO_speed_mask << (position * 2U));
            temp |= (((settings->speed) & GPIO_speed_mask) << (position * 2U));
            port->OSPEEDR = temp; 
        }

        /* 3. Set pull-up or pull-down */
        temp = port->PUPDR;
        temp &= ~(GPIO_pupd_mask << (position * 2U));
        temp |= (((settings->pull) & GPIO_pupd_mask) << (position * 2U));
        port->PUPDR = temp;

        /* 4. Set alternate function */
        if ((settings->mode & GPIO_mode_mask) == GPIO_mode_alternate)
        {
            if (position < 8U)
            {
                temp = port->AFR[0];
                temp &= ~(GPIO_af_mask  << (position * 4U));
                temp |= (((settings->alternate) & GPIO_af_mask) << (position * 4U));
                port->AFR[0] = temp;
            }
            else
            {
                temp = port->AFR[1];
                temp &= ~(GPIO_af_mask << ((position - 8U) * 4U));
                temp |= (((settings->alternate) & GPIO_af_mask) << ((position - 8U) * 4U));
                port->AFR[1] = temp;
            }
        }
    }
}


int gpio_read(GPIO_TypeDef *port, uint16_t pin)
{
    return ((port->IDR & pin) != 0U);
}

void gpio_set(GPIO_TypeDef *port, uint16_t pin)
{
    port->BSRR = (uint32_t)pin;
}

void gpio_reset(GPIO_TypeDef *port, uint16_t pin)
{
    port->BRR = (uint32_t)pin;
}

void gpio_toogle(GPIO_TypeDef *port, uint16_t pin)
{
    if (gpio_read(port, pin))
    {
        port->BRR = (uint32_t)pin;
    }
    else
    {
        port->BSRR = (uint32_t)pin;
    }
}