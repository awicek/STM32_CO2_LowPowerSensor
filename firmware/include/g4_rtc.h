#ifndef G4_RTC_H_
#define G4_RTC_H_


#include <stm32g4xx_ll_rtc.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include "stm32g4xx.h"
#include <stdint.h>

/**
 *  @brief Handles the RTC interrupt. 
 */
void rtc_it_handler();

/**
 *  @brief Enables the RTC pheripheral
 *  Clock source is LSI. 
 *  Enables the RTC interrupt.
 *  Does not configure defuult time and date.
 */
void rtc_init()
{
    /* Set RTC kernel clock source  */ 
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    /* Enable RTC kernel clock*/
    LL_RCC_EnableRTC();
    /* Enable RTC Register interface clockk  */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTCAPB);

    LL_RTC_InitTypeDef rtc_init;
    LL_RTC_Init(RTC,  &rtc_init);
    // LL_RTC_BKP_GetRegister(RTC, LL_RTC_BKP_DR0) = 0x32F2; 

}

void rtc_wakup_disable();

/**
 *  @brief Sets the RTC wakeup timer to trigger an interrupt after 
 */
void rtc_wakeup_init(uint16_t wakeup_time_10ms)
{
    /* Enable wakeup  WUTE */ 
    LL_RTC_WAKEUP_Enable(RTC);
    LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_16);
    LL_RTC_WAKEUP_SetAutoReload(RTC, 0);
    LL_RTC_DisableAlarmPullUp(RTC);
    LL_RTC_EnableIT_WUT(RTC);
}

/**
 *  @brief Check for the RTC wakeup flag. RTC_SR register
 */
void rtc_is_wakup_flag_set();

/**
 *  @brief Clear the RTC wakeup flag. RTC_SCR register 
 */
void rtc_claer_wakeup_flag(); 

void rtc_enter_init_mode();

void rtc_exit_init_mode();

void rtc_enable_write_protection();

void rtc_disable_write_protection();

#endif /* G4_RTC_H_ */
