#include "Impulse.h"
#include <atmel_start.h>
#include "SQM_Defines.h"


#define IMPULSE_CHANNEL_COUNT 3

static void Impulse_Counting_Channel_1(void);
static void Impulse_Counting_Channel_2(void);
static void Impulse_Counting_Channel_3(void);

uint32_t impulse_counter[IMPULSE_CHANNEL_COUNT];
bool impulse_block[IMPULSE_CHANNEL_COUNT];

static struct timer_task TIMER_2_task1;
static struct timer_task TIMER_2_task2;
static struct timer_task TIMER_2_task3;
static void TIMER_2_task1_cb(const struct timer_task *const timer_task);
static void TIMER_2_task2_cb(const struct timer_task *const timer_task);
static void TIMER_2_task3_cb(const struct timer_task *const timer_task);


//-------------------------------------------------------------------------------------------------
void Impulse_Init(void)
{
	int i;
	
	for(i=0;i<IMPULSE_CHANNEL_COUNT;i++)
	{
		impulse_block[i] = false;
		impulse_counter[i] = 0;
	}

	ext_irq_register(PIN_PA10, Impulse_Counting_Channel_1);
	ext_irq_register(PIN_PA11, Impulse_Counting_Channel_2);
	ext_irq_register(PIN_PA09, Impulse_Counting_Channel_3);	
	ext_irq_enable(PIN_PA10);
	ext_irq_enable(PIN_PA11);
	ext_irq_enable(PIN_PA09);	
}
//-------------------------------------------------------------------------------------------------
uint32_t Get_Impluse_Counter(int channel)
{
	if(channel == 0 || channel > IMPULSE_CHANNEL_COUNT)
		return 0;
	else
		channel--;
	
	uint32_t value = impulse_counter[channel];

#ifdef IMPULSE_COUNTER_CLEAR_AT_READ
	impulse_counter[channel] = 0;
#endif

	return value;
}
//-------------------------------------------------------------------------------------------------
static void Impulse_Counting_Channel_1(void)
{
	if(impulse_block[0])
		return;
	impulse_counter[0]++;

	TIMER_2_task1.interval = IMPULSE_BLOCK_TIME;
	TIMER_2_task1.cb       = TIMER_2_task1_cb;
	TIMER_2_task1.mode     = TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER_2, &TIMER_2_task1);

	if(impulse_block[1] == false && impulse_block[2] == false)
		timer_start(&TIMER_2);

	impulse_block[0] = true;
}
//-------------------------------------------------------------------------------------------------
static void Impulse_Counting_Channel_2(void)
{
	if(impulse_block[1])
		return;
	impulse_counter[1]++;

	TIMER_2_task2.interval = IMPULSE_BLOCK_TIME;
	TIMER_2_task2.cb       = TIMER_2_task2_cb;
	TIMER_2_task2.mode     = TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER_2, &TIMER_2_task2);

	if(impulse_block[0] == false && impulse_block[2] == false)
		timer_start(&TIMER_2);

	impulse_block[1] = true;
}
//-------------------------------------------------------------------------------------------------
static void Impulse_Counting_Channel_3(void)
{
	if(impulse_block[2])
		return;
	impulse_counter[2]++;

	TIMER_2_task3.interval = IMPULSE_BLOCK_TIME;
	TIMER_2_task3.cb       = TIMER_2_task3_cb;
	TIMER_2_task3.mode     = TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER_2, &TIMER_2_task3);

	if(impulse_block[0] == false && impulse_block[1] == false)
		timer_start(&TIMER_2);

	impulse_block[2] = true;
}
//-------------------------------------------------------------------------------------------------
static void TIMER_2_task1_cb(const struct timer_task *const timer_task)
{
	impulse_block[0] = false;

	if(impulse_block[1] == false && impulse_block[2] == false)
		timer_stop(&TIMER_2);
}
//-------------------------------------------------------------------------------------------------
static void TIMER_2_task2_cb(const struct timer_task *const timer_task)
{
	impulse_block[1] = false;

	if(impulse_block[0] == false && impulse_block[2] == false)
		timer_stop(&TIMER_2);
}
//-------------------------------------------------------------------------------------------------
static void TIMER_2_task3_cb(const struct timer_task *const timer_task)
{
	impulse_block[2] = false;

	if(impulse_block[0] == false && impulse_block[1] == false)
		timer_stop(&TIMER_2);
}
//-------------------------------------------------------------------------------------------------

