#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "Common.h"

typedef struct  
{
	uint8_t	tm;
	uint8_t	alpha;	
//	uint8_t	delta;		
//	uint8_t	p_min;
//	uint8_t	p_max;	
//	uint8_t	p_alert_min;		
//	uint8_t	p_alert_max;			
	uint8_t	control;				
	uint8_t	crc;					
}CONFIGURATION;

void Configuration_Init(void);

//extern CONFIGURATION configuration;
extern const CONFIGURATION* const configuration;

#endif /* CONFIGURATION_H_ */