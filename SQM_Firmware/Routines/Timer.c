#include "Timer.h"
#include <atmel_start.h>


static uint32_t msTicks = 0;
static struct timer_task TIMER_1_task1;
static void TIMER_1_task1_cb(const struct timer_task *const timer_task);

//-------------------------------------------------------------------------------------------------
void Timer_Init(void)
{
	msTicks = 0;

	TIMER_1_task1.interval = 1;
	TIMER_1_task1.cb       = TIMER_1_task1_cb;
	TIMER_1_task1.mode     = TIMER_TASK_REPEAT;
	timer_add_task(&TIMER_1, &TIMER_1_task1);
	timer_start(&TIMER_1);
}
//-------------------------------------------------------------------------------------------------
static void TIMER_1_task1_cb(const struct timer_task *const timer_task)
{
	msTicks++;
}
//-------------------------------------------------------------------------------------------------
uint64_t Get_Timer_Millisec(void)
{
	return msTicks;
}
//-------------------------------------------------------------------------------------------------
TIMER_T SetTimeout(TIMER_T timing)
{
  timing += msTicks;
  if (timing == 0) timing++; // ensure return value not 0, would mean disabled timer
  return(timing);
}
//-------------------------------------------------------------------------------------------------
bool AfterTime(TIMER_T timer)
{
	if (timer == 0) return(false); // disabled timer
	return((msTicks >= timer) ? true:false);
}
//-------------------------------------------------------------------------------------------------
void Timer_Delay_ms(uint32_t timeout_ms)
{
	TIMER_T	timer_delay = SetTimeout(timeout_ms);
	while(!AfterTime(timer_delay));
	
	return;
}
//-------------------------------------------------------------------------------------------------
