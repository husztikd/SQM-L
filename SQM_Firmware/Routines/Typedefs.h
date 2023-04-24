/*
 * typedefs.h
 *
 * Created: 2021. 03. 19. 11:32:57
 *  Author: hubaja2
 */ 
#include "defines.h"

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_


typedef struct {
	uint8_t config;
	uint8_t alfa; 	
	uint8_t* measurement;
	uint8_t Tmin; // minutes
	uint8_t int_warning_state;
	uint8_t water_sensor_state;
	uint8_t delta;
	//uint8_t impulse;
	uint8_t vbat[2];
	uint8_t signal_strength;
}payloadStruct;

typedef struct{
	uint8_t config;
	uint8_t Tmin;
	uint8_t int_warning_state;
	uint8_t water_sensor_state;
}warningPayloadStruct;

typedef struct{
	uint8_t config;
	uint8_t water_sensor_state;
	//uint8_t hydrant_action;
	//uint8_t nfc_tag[8];
}waterSensorPayloadStruct;

typedef struct{
	uint8_t Tmin;
	uint8_t Alfa;
	uint8_t Delta;
	uint8_t	Pmin_limit;
	uint8_t Pmin_threshold;
	uint8_t Pmax_threshold;
	uint8_t Pmax_limit;
	uint8_t ControlByte;
	
}configStruct;

typedef enum{
	LOWER_LIMIT,
	LOWER_THRESHOLD,
	NORMAL,
	UPPER_THRESHOLD,
	UPPER_LIMIT
}OperatingState;

typedef enum{
	HYDRANT_CLOSED,
	HYDRANT_OPENED
	}HydrantState;


#endif /* TYPEDEFS_H_ */