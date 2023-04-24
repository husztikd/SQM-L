#ifndef RN2483_H_
#define RN2483_H_

#include "Common.h"

typedef union
{
	struct
	{
		unsigned char found : 1;
		unsigned char connected : 1;
	};
	uint32_t all;
}STATUS_FLAGS;


typedef union
{
	struct
	{
		unsigned char mac_state : 4;
		unsigned char join_status : 1;
		unsigned char automatic_reply_status : 1;
		unsigned char adr_status : 1;
		unsigned char silent_status : 1;
		unsigned char pause_status : 1;
		unsigned char rfu : 1;
		unsigned char link_status : 1;
		unsigned char channel_updated : 1;
		unsigned char output_power_updated : 1;
		unsigned char nbrep_updated : 1;
		unsigned char prescaler_updated : 1;
		unsigned char second_receive_window_updated : 1;
		unsigned char tx_timing_updated : 1;
	};
	uint16_t all;
}RN2483_STATUS_FLAGS;


bool RN2483_Init(void);
bool RN2483_Connect(void);
void RN2483_Sleep(void);
bool RN2483_Wake(void);
bool RN2483_Reset(void);
bool RN2483_Send_Packet(uint8_t port, uint8_t *packet, uint16_t packet_size);
bool RN2483_Send_Packet_Acknowledged(uint8_t port, uint8_t *packet, uint16_t packet_size);
bool Is_RN2483_Found(void);
bool Is_RN2483_Connected(void);
char* RN2483_Get_Deveui_Value(void);
int RN2483_Get_Voltage_Value(void);

#endif /* RN2483_H_ */