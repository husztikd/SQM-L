#include "Watchdog.h"
#include <atmel_start.h>
#include "Timer.h"

#define TIME_RESET_IMP 10

//-------------------------------------------------------------------------------------------------
void Watchdog_Reset(void)
{
	gpio_set_pin_level(DONE, true);
	Timer_Delay_ms(TIME_RESET_IMP);
	gpio_set_pin_level(DONE, false);	
}
//-------------------------------------------------------------------------------------------------