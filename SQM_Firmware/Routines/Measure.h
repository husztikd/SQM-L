#ifndef MEASURE_H_
#define MEASURE_H_

#include "Common.h"

	void Measure_Init(void);
	void Measure(void);
	void Set_Measure_Time(uint16_t time_measure);
	bool Get_Measure_Need_To_Send(void);	

#endif /* MEASURE_H_ */