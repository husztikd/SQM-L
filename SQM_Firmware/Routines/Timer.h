#ifndef TIMER_H_
#define TIMER_H_

#include "Common.h"

typedef uint32_t TIMER_T;

	void Timer_Init(void);
//	uint64_t Get_Timer_Millisec(void);	
	TIMER_T SetTimeout(TIMER_T timing);
	bool AfterTime(TIMER_T timer);
	void Timer_Delay_ms(uint32_t timeout_ms);
#endif /* TIMER_H_ */