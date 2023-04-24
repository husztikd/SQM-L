#ifndef LORA_H_
#define LORA_H_

#include "Common.h"

typedef union
{
	struct
	{
		unsigned char found : 1;
		unsigned char connected : 1;
	};
	uint32_t all;
}LORA_STATUS_FLAGS;


bool Lora_Init(void);
bool Lora_Reset(void);
bool Lora_Connect(void);
void Lora_Sleep(void);
bool Lora_Wake(void);
bool Lora_Send(uint8_t port, uint8_t *packet, uint16_t packet_size);
bool Is_Lora_Connected(void);

#endif /* LORA_H_ */