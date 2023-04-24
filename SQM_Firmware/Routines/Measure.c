#include "Measure.h"
#include <atmel_start.h>
#include "Configuration.h"

#define TIMER_INTERVAL 0.5	// in sec
#define MEASURE_INTERVAL 1			// in sec

static struct timer_task TIMER_0_task1;
static void TIMER_0_task1_cb(const struct timer_task *const timer_task);

uint16_t measure_timer;


//-------------------------------------------------------------------------------------------------
void Measure_Init(void)
{
	TIMER_0_task1.interval = 1;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_start(&TIMER_0);
	
	Set_Measure_Time(configuration->tm);			
}
//-------------------------------------------------------------------------------------------------
void Measure(void)
{
	Set_Measure_Time(configuration->tm);			
}
//-------------------------------------------------------------------------------------------------
void Set_Measure_Time(uint16_t time_measure)
{
	measure_timer = time_measure * (MEASURE_INTERVAL/TIMER_INTERVAL);
	TIMER_0.time = 0;
}
//-------------------------------------------------------------------------------------------------
bool Get_Measure_Need_To_Send(void)
{
	if(measure_timer)
		return false;
	return true;
}
//-------------------------------------------------------------------------------------------------
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	if(measure_timer)
		measure_timer--;
}
//-------------------------------------------------------------------------------------------------
