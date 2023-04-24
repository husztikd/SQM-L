#include "Lora.h"
#include "RN2483.h"


#define SEND_ACKNOWLEDGE 10
uint8_t send_anknowledge;



bool Lora_Init(void)
{
	send_anknowledge = 0;
	
	return RN2483_Init();
}
//-------------------------------------------------------------------------------------------------
bool Lora_Reset(void)
{
	return RN2483_Reset();	
}
//-------------------------------------------------------------------------------------------------
bool Lora_Connect(void)
{
	return RN2483_Connect();
}
//-------------------------------------------------------------------------------------------------
void Lora_Sleep(void)
{
	RN2483_Sleep();
}
//-------------------------------------------------------------------------------------------------
bool Lora_Wake(void)
{
	return RN2483_Wake();
}
//-------------------------------------------------------------------------------------------------
bool Lora_Send(uint8_t port, uint8_t *packet, uint16_t packet_size)
{
	if(Is_RN2483_Connected())
	{
		if(send_anknowledge)
		{
			send_anknowledge--;
			return RN2483_Send_Packet(port, packet, packet_size);
		}
		else
		{
			send_anknowledge = SEND_ACKNOWLEDGE-1;
			return RN2483_Send_Packet_Acknowledged(port, packet, packet_size);
		}
	}

	return false;	
}
//-------------------------------------------------------------------------------------------------
bool Is_Lora_Connected(void)
{
	return Is_RN2483_Connected();
}
//-------------------------------------------------------------------------------------------------

