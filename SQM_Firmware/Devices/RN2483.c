#include "RN2483.h"
#include <atmel_start.h>
#include "Uart.h"
#include "Timer.h"
#include "Debug.h"
#include "SQM_Defines.h"

#define BUFF_SIZE 256
#define TIMEOUT_PARAMETER 5000
#define TIMEOUT_GET_DATA 100
#define TIMEOUT_RESET 4000
#define TIMEOUT_CONNECTION 30000
#define TIMEOUT_GET_STATUS 3000
#define TIMEOUT_SEND 10000

#define RETRY_RESET 3
#define RETRY_CONNECT 3
#define RETRY_WAKE 3

#define CHANNEL_ACTIVE 8
#define CHANNEL_MAX 15
#define DUTY_CYCLE_PERCENT 1
#define DUTY_CYCLE 799

#define BUFFER_LENGTH 256
#define SEND_DATA_PAYLOAD_MAX_LENGTH (BUFFER_LENGTH - 17 - 2 - 1)



#define TIME_RESET 100

char devaddr[50];

char deveui[100];
char voltage[100];

static char response[BUFF_SIZE];
char buffer[SEND_DATA_PAYLOAD_MAX_LENGTH];
char data_hex_buffer[SEND_DATA_PAYLOAD_MAX_LENGTH];

TIMER_T timeout;
STATUS_FLAGS status_flags;
RN2483_STATUS_FLAGS module_status;

uint32_t freq = 867100000;

void RN2483_Setup(void);

bool RN2483_Set_Frequency(void);
bool RN2483_Set_Duty_Cycle(void);
bool RN2483_Set_Data_Rate_Range(void);
bool RN2483_Set_Channel_Enable(void);

char* RN2483_Get_Hweui(void);
char* RN2483_Get_Deveui(void);
char* RN2483_Get_Appeui(void);
char* RN2483_Get_Sysver(void);
char* RN2483_Get_Rssi(void);
char* RN2483_Get_Snr(void);
bool RN2483_Get_Status(void);
char* RN2483_Get_Voltage(void);

bool RN2483_Set_Parameter(char* str, char* resp);
char* RN2483_Get_Parameter(char* str);
void RN2483_Send_Command(char* cmd);
bool RN2483_Get_Data(void);
bool RN2483_Get_Line(void);
bool RN2483_Response_Equal(char *str);
bool RN2483_Response_Contain(char *str);


//-------------------------------------------------------------------------------------------------
bool RN2483_Init(void)
{
	status_flags.all = 0;
	module_status.all = 0;
	
	RN2483_Reset_Pin_High();
	
	RN2483_Reset();
	RN2483_Setup();
	RN2483_Connect();
	RN2483_Sleep();
	

	return status_flags.found;
}
//-------------------------------------------------------------------------------------------------
void RN2483_Setup(void)
{
	char command[256];
	
	if(status_flags.found == false)
		return;

	 Debug_Printf("RN2483 Setuping...\n");

	RN2483_Set_Parameter("mac reset 868", "ok");
	RN2483_Get_Status();

  if(RN2483_Set_Parameter("mac set class a", "ok"))
		Debug_Printf("Class A Set\n");
  if(RN2483_Set_Parameter("mac set adr on", "ok"))
		Debug_Printf("ADR Off Set\n");
  if(RN2483_Set_Parameter("mac set dr 0", "ok"))				// DR0 = SF12
		Debug_Printf("DR 0 Set\n");

  if(RN2483_Set_Parameter("mac set retx 0", "ok"))
	  Debug_Printf("Retx 0 Set\n");
  if(RN2483_Set_Parameter("mac set pwridx 1", "ok"))				
	  Debug_Printf("Output Power Set\n");
  if(RN2483_Set_Parameter("mac set ar off", "ok"))
	  Debug_Printf("Output Power Set\n");




  strcpy(deveui, RN2483_Get_Hweui());

  sprintf(command, "mac set deveui %s", deveui);
	if(RN2483_Set_Parameter(command, "ok"))
		Debug_Printf("Deveui set\n");
  sprintf(command, "mac set appeui %s", APPEUI);		
	if(RN2483_Set_Parameter(command, "ok"))
		Debug_Printf("Appeui set\n");
	sprintf(command, "mac set appkey %s", APPKEY);
	if(RN2483_Set_Parameter(command, "ok"))
		Debug_Printf("Appkey set\n");

  if(RN2483_Set_Parameter("mac save", "ok"))
		Debug_Printf("save\n");

  RN2483_Reset();

	Debug_Printf("RN2483 Initialized\n");		
  Debug_Printf("Hweui: %s\n", RN2483_Get_Hweui());
  Debug_Printf("Deveui: %s\n", RN2483_Get_Deveui());
  Debug_Printf("Appeui: %s\n", RN2483_Get_Appeui());	
  Debug_Printf("Sysver: %s\n", RN2483_Get_Sysver());
  Debug_Printf("Rssi: %s\n", RN2483_Get_Rssi());
  Debug_Printf("Snr: %s\n", RN2483_Get_Snr());
  Debug_Printf("Voltage: %s\n", RN2483_Get_Voltage());

}
//-------------------------------------------------------------------------------------------------
bool RN2483_Reset(void)
{
	TIMER_T reset_timeout;
	int retry = RETRY_RESET;
	char* str = NULL;

	status_flags.found = false;
	status_flags.connected = false;

	while(retry--)
	{
	Debug_Printf("RN2483 Reseting...\n");
		memset(response, 0, sizeof(response));
		
		RN2483_Reset_Pin_Low();
		Timer_Delay_ms(TIME_RESET);
		RN2483_Reset_Pin_High();
		
		reset_timeout = SetTimeout(TIMEOUT_RESET);		
		while(!AfterTime(reset_timeout))
		{
			if(RN2483_Get_Line())
			{
				for(int i=0;i<BUFF_SIZE;i++)
				{
					if(response[i] == 'R')
					{
						str = &response[i];
						break;
					}
				}
				if(strstr(str, "RN2483"))
				{
					Debug_Printf("RN2483 Reseted\n");
					status_flags.found = true;
					return true;
				}
			}
		}
	}

	status_flags.found = false;
	Debug_Printf("RN2483 not found\n");

	return false;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Connect(void)
{
	int retry = RETRY_CONNECT;

	RN2483_Get_Status();
	status_flags.connected = module_status.join_status;
	if(status_flags.found == true && status_flags.connected == false)
	{
		Debug_Printf("RN2483 Connecting...\n");
		while(retry--)
		{
			timeout = SetTimeout(TIMEOUT_CONNECTION);

			RN2483_Send_Command("mac join otaa");

			while(!AfterTime(timeout))
			{
				if(RN2483_Get_Line())
				{
					if(RN2483_Response_Equal("accepted"))
					{
						Debug_Printf("RN2483 Connected\n");
						status_flags.connected = true;
						return status_flags.connected;
					}
					if(RN2483_Response_Equal("denied"))
					{
						Debug_Printf("Denied");
						break;
					}
					if(RN2483_Response_Equal("no_free_ch"))
					{
						Debug_Printf("No free channel");
						break;
					}
				}
			}
		}
		Debug_Printf("RN2483 Connect Fail...\n");
	}
	return status_flags.connected;	
}
//-------------------------------------------------------------------------------------------------
void RN2483_Sleep(void)
{
	RN2483_Set_Parameter("radio rxstop", "ok");
	RN2483_Send_Command("sys sleep 200000000");
	Debug_Printf("RN2483 Sleep\n");
	Timer_Delay_ms(500);
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Wake(void)
{
	int retry = RETRY_WAKE;	
	uint8_t data = 0x55;

	Debug_Printf("RN2483 Waking\n");
	while(retry--)
	{
		Uart_Break();
		Uart_Write_Bytes(&data, 1);
		Timer_Delay_ms(10);

		if(RN2483_Get_Status())
		{
			status_flags.found =  true;
			if(module_status.join_status == false)
				RN2483_Connect();
			strcpy(voltage, RN2483_Get_Voltage());
			Debug_Printf("RN2483 Wake\n");	
			return true;
		}
		else
			RN2483_Reset();				// BUG - Workaround, because after 4-5times never wake up again
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Send_Packet(uint8_t port, uint8_t *packet, uint16_t packet_size)
{
	TIMER_T send_timeout;
	
	if(status_flags.connected && packet_size < SEND_DATA_PAYLOAD_MAX_LENGTH)
	{
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "mac tx uncnf %d %s", port, (char*)packet);
		send_timeout = SetTimeout(TIMEOUT_SEND);
		RN2483_Send_Command(buffer);

		while(!AfterTime(send_timeout))
			if(RN2483_Get_Line())
				if(RN2483_Response_Equal("mac_tx_ok"))
				{
					Debug_Printf("LoRa Sent ucnf\n");
					return true;
				}
	}
	Debug_Printf("LoRa Send failed!!!\n");					
	return false;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Send_Packet_Acknowledged(uint8_t port, uint8_t *packet, uint16_t packet_size)
{
	TIMER_T send_timeout;

	if(status_flags.connected && packet_size < SEND_DATA_PAYLOAD_MAX_LENGTH)
	{
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "mac tx cnf %d %s", port, (char*)packet);
		send_timeout = SetTimeout(TIMEOUT_SEND);
		RN2483_Send_Command(buffer);

		while(!AfterTime(send_timeout))
			if(RN2483_Get_Line())
				if(RN2483_Response_Equal("mac_tx_ok"))
				{
					Debug_Printf("LoRa Sent cnf\n");			
					return true;			
				}
		
	}
	Debug_Printf("LoRa Send failed!!!\n");				
	return false;
}
//-------------------------------------------------------------------------------------------------
bool Is_RN2483_Found(void)
{
	return status_flags.found; 
}
//-------------------------------------------------------------------------------------------------
bool Is_RN2483_Connected(void)
{
	return status_flags.connected;
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Deveui_Value(void)
{
	return deveui;
}
//-------------------------------------------------------------------------------------------------
int RN2483_Get_Voltage_Value(void)
{
	char* str = voltage;
	if(str)
		return atoi(str);
	else
		return 0;		
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Set_Frequency(void)
{
	uint8_t channel = 0, offset = 3;
	const uint32_t channel_frequency_start = 867100000;
	const uint32_t channel_frequency_step = 200000;
	char str[100];
	bool ret = true;

	for(channel = offset; channel < CHANNEL_ACTIVE; channel++)
	{
		sprintf(str, "mac set ch freq %d %lu", channel, channel_frequency_start + channel_frequency_step * (channel - offset));
		if( !RN2483_Set_Parameter(str, "ok") )
		ret = false;
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Set_Duty_Cycle(void)
{
	uint8_t channel;
	char str[100];
	bool ret = true;

	for(channel = 0; channel < CHANNEL_ACTIVE; channel++)
	{
		sprintf(str, "mac set ch dcycle %u %u", channel, DUTY_CYCLE);
		if( !RN2483_Set_Parameter(str, "ok") )
		ret = false;
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Set_Data_Rate_Range(void)
{
	uint8_t channel;
	char str[100];
	bool ret = true;

	for(channel = 0; channel < CHANNEL_ACTIVE; channel++)
	{
		if(channel == 1)
		{
			sprintf(str, "mac set ch drrange %u 0 6", channel);
			if( !RN2483_Set_Parameter(str, "ok") )
			ret = false;
		}
		if(channel > 2)
		{
			sprintf(str, "mac set ch drrange %u 0 5", channel);
			if( !RN2483_Set_Parameter(str, "ok") )
			ret = false;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Set_Channel_Enable(void)
{
	uint8_t channel;
	char str[100];
	bool ret = true;

	for(channel = 0; channel < CHANNEL_ACTIVE; channel++)
	{
		sprintf(str, "mac set ch status %u on", channel);
		if( !RN2483_Set_Parameter(str, "ok") )
		ret = false;
	}

	return ret;
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Hweui(void)
{
	return RN2483_Get_Parameter("sys get hweui");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Appeui(void)
{
	return RN2483_Get_Parameter("mac get appeui");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Deveui(void)
{
	return RN2483_Get_Parameter("mac get deveui");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Sysver(void)
{
	return RN2483_Get_Parameter("sys get ver");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Rssi(void)
{
	return RN2483_Get_Parameter("radio get rssi");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Snr(void)
{
	return RN2483_Get_Parameter("radio get snr");
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Voltage(void)
{
	memset(voltage, 0, 100);
	strcpy(voltage, RN2483_Get_Parameter("sys get vdd"));
	return voltage;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Get_Status(void)
{
	uint32_t num;
	TIMER_T get_status_timeout;	
	module_status.all = 0;
	
	
	memset(response, 0, sizeof(response));	
	get_status_timeout = SetTimeout(TIMEOUT_GET_STATUS);
	
	RN2483_Send_Command("mac get status");
	
	while(!AfterTime(get_status_timeout))
	{
		if(RN2483_Get_Line())
		{
			if(strlen(response) == 8)
			{
				sscanf(response, "%x", (int*)&num);
				module_status.all = num;
		
				return true;
			}
		}
	
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Set_Parameter(char* str, char* resp)
{
	bool responded = false;
	memset(response, 0, sizeof(response));
	RN2483_Send_Command(str);

	if(RN2483_Get_Data())
		if(strstr(response, resp))
			responded = true;
	return responded;
}
//-------------------------------------------------------------------------------------------------
char* RN2483_Get_Parameter(char* str)
{
	memset(response, 0, sizeof(response));
	RN2483_Send_Command(str);
	timeout = SetTimeout(TIMEOUT_PARAMETER);

	while(!RN2483_Get_Line())
	{
		if(AfterTime(timeout))
		{
			response[0] = '\0';
			break;
		}
	}
	//Debug_Printf(response);
	return response;
}
//-------------------------------------------------------------------------------------------------
void RN2483_Send_Command(char* cmd)
{
	char str[BUFF_SIZE];
	strcpy(str, cmd);
	strcat(str,"\r\n");
	Uart_Write_Bytes((uint8_t*)str, strlen(str));
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Get_Data(void)
{
	uint8_t c;
	TIMER_T timeout_data;
	bool new_data = false;
	uint16_t ptr = 0;

	timeout_data = SetTimeout(TIMEOUT_PARAMETER);

	while(1)
	{
		while(Get_Uart_Byte(&c))
		{
			response[ptr] = c;
			ptr++;
			new_data = true;
			if(c == '\n')
			{
				return new_data;
			}
			timeout_data = SetTimeout(TIMEOUT_GET_DATA);
		}
		if(AfterTime(timeout_data))
		break;
	}
	return new_data;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Get_Line(void)
{
	uint8_t c;
	static bool new_data = true;
	static uint16_t buff_p = 0;

	if(new_data)
	{
		new_data = false;
		buff_p = 0;
		memset(response, 0, sizeof(response));
	}

	while(Get_Uart_Byte(&c))
	{
		Timer_Delay_ms(1);
		switch(c)
		{
			case '\n':
			new_data = true;
			//Debug_Printf("LR: %s\n",response);
			return true;
			case '\r':
			response[buff_p] = '\0';
			break;
			default:
			response[buff_p] = c;
			if(buff_p < (sizeof(response) - 1))
			buff_p++;
		};
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Response_Equal(char *str)
{
	return !strcmp(response, str);
}
//-------------------------------------------------------------------------------------------------
bool RN2483_Response_Contain(char *str)
{
	if(strstr(response, str) != NULL)
		return true;
	return false;
}
//-------------------------------------------------------------------------------------------------

