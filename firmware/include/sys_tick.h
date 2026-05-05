#ifndef SYS_TICK_H_
#define SYS_TICK_H_

#include "stm32g474xx.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t current_tick;

__STATIC_INLINE uint32_t sys_tick_get(void)
{
  return current_tick;
}

__STATIC_INLINE void sys_tick_increment(void)
{
  current_tick++;
}

/**
 *  @brief Delay execution for given number of millisends.
 *  Do not call from interrups.
 *  @param ms Number of milliseconds to delay.
 */
void sys_delay(uint32_t ms);

#ifdef __cplusplus
}
#endif
#endif // SYS_TICK_H_
