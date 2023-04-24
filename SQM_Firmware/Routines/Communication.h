#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "Common.h"
#include "SQM_Defines.h"

typedef struct {
	uint16_t payload_counter;
	uint8_t control;	
	uint8_t time_measure;
	uint16_t battery;
	uint8_t signal_quality;
	char radio_version[6];
	uint16_t device_fw_version;
}sPayLoad_Start;


typedef struct  
{
	uint16_t payload_counter;
	uint8_t control;
	uint8_t warning;
	uint32_t impulse_1;
	uint32_t impulse_2;
	uint32_t impulse_3;		
	uint16_t battery;
	uint8_t signal_quality;
	uint8_t time_measure;
}sPayLoad_Measure;


void Communication(void);

#endif /* COMMUNICATION_H_ */