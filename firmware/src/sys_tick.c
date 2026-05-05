#include "sys_tick.h"


volatile uint32_t current_tick = 0;


void sys_delay(uint32_t ms)
{
  uint32_t start_tick = sys_tick_get();
  while ((sys_tick_get() - start_tick) < ms) {};
}
