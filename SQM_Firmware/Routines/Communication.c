#include "Communication.h"
#include "Common.h"
#include "Lora.h"
#include "Impulse.h"
#include "Configuration.h"


#define PAYLOAD_NORMAL	0x00
#define PAYLOAD_ALERT		0x40
#define PAYLOAD_START		0xC0

static uint16_t payload_counter = 0;
static bool need_to_send_start_packet = true;

sPayLoad_Start payload_start;
sPayLoad_Measure payload_measure;

void Send_Payload_Start(void);
void Get_Payload_Start(void);
uint16_t Serialize_Payload_Start(uint8_t* buffer, uint16_t *size, sPayLoad_Start* payload);

void Send_Payload_Measure(void);
void Get_Payload_Measure_Data(void);
int Serialize_Payload_Measure(uint8_t* buffer, uint16_t *size, sPayLoad_Measure* payload);


int Convert_Payload_to_Hex_Format(uint8_t* payload, uint16_t payload_size, uint8_t* payload_hex, uint16_t* payload_hex_size);


//-------------------------------------------------------------------------------------------------
void Communication(void)
{

	Lora_Wake();
	
	if(need_to_send_start_packet)
		Send_Payload_Start();


	Send_Payload_Measure();
	
	Lora_Sleep();
}

//-------------------------------------------------------------------------------------------------
void Send_Payload_Start(void)
{
	uint8_t payload_buffer[PAYLOAD_BUFFER/2-1] = {0};			// /2 - half size because of HEX format; -1 string end.
	uint8_t payload_hex_buffer[PAYLOAD_BUFFER-1] = {0};
	uint16_t payload_size, payload_hex_size;

	Get_Payload_Start();
	Serialize_Payload_Start(payload_buffer, &payload_size, &payload_start);
	Convert_Payload_to_Hex_Format(payload_buffer, payload_size, payload_hex_buffer, &payload_hex_size);
	Lora_Send(1, payload_hex_buffer, payload_hex_size);
	
	need_to_send_start_packet = false;
}
//-------------------------------------------------------------------------------------------------
void Get_Payload_Start(void)
{
	payload_start.payload_counter = payload_counter;
	payload_start.control = PAYLOAD_START;	
	payload_start.time_measure = configuration->tm;
	payload_start.device_fw_version = SOFTWARE_VERSION_NUM;
		
	payload_counter++;
}
//-------------------------------------------------------------------------------------------------
uint16_t Serialize_Payload_Start(uint8_t* buffer, uint16_t *size, sPayLoad_Start* payload)
{
	uint16_t i = 0;

	memcpy(&buffer[i], &payload->payload_counter, sizeof payload->payload_counter);
	i += sizeof payload->payload_counter;
	memcpy(&buffer[i], &payload->control, sizeof payload->control);
	i += sizeof payload->control;
	memcpy(&buffer[i], &payload->time_measure, sizeof payload->time_measure);
	i += sizeof payload->time_measure;
	memcpy(&buffer[i], &payload->battery, sizeof payload->battery);
	i += sizeof payload->battery;
	memcpy(&buffer[i], &payload->signal_quality, sizeof payload->signal_quality);
	i += sizeof payload->signal_quality;
	memcpy(&buffer[i], &payload->radio_version, 6);
	i += 6;
	memcpy(&buffer[i], &payload->device_fw_version, sizeof payload->device_fw_version);
	i += sizeof payload->device_fw_version;

	*size = i;
	
	return 0;
}
//-------------------------------------------------------------------------------------------------
void Send_Payload_Measure(void)
{
	uint8_t payload_buffer[PAYLOAD_BUFFER/2-1] = {0};			// /2 - half size because of HEX format; -1 string end.
	uint8_t payload_hex_buffer[PAYLOAD_BUFFER-1] = {0};
	uint16_t payload_size, payload_hex_size;

	Get_Payload_Measure_Data();
	Serialize_Payload_Measure(payload_buffer, &payload_size, &payload_measure);
	Convert_Payload_to_Hex_Format(payload_buffer, payload_size, payload_hex_buffer, &payload_hex_size);
	Lora_Send(1, payload_hex_buffer, payload_hex_size);	
}
//-------------------------------------------------------------------------------------------------
void Get_Payload_Measure_Data(void)
{
	payload_measure.payload_counter = payload_counter; 
	payload_measure.control = PAYLOAD_NORMAL;
	payload_measure.warning = 0; //Get_Warning_State();
	payload_measure.impulse_1 = Get_Impluse_Counter(1);
	payload_measure.impulse_2 = Get_Impluse_Counter(2);
	payload_measure.impulse_3 = Get_Impluse_Counter(3);		
	payload_measure.time_measure = configuration->tm;

	payload_counter++;
}
//-------------------------------------------------------------------------------------------------
int Serialize_Payload_Measure(uint8_t* buffer, uint16_t *size, sPayLoad_Measure* payload)
{
	uint16_t i = 0;


	memcpy(&buffer[i], &payload->payload_counter, sizeof payload->payload_counter);
	i += sizeof payload->payload_counter;

	memcpy(&buffer[i], &payload->control, sizeof payload->control);
	i += sizeof payload->control;

	memcpy(&buffer[i], &payload->warning, sizeof payload->warning);
	i += sizeof payload->warning;

	memcpy(&buffer[i], &payload->impulse_1, sizeof payload->impulse_1);
	i += sizeof payload->impulse_1;

	memcpy(&buffer[i], &payload->impulse_2, sizeof payload->impulse_2);
	i += sizeof payload->impulse_2;

	memcpy(&buffer[i], &payload->impulse_3, sizeof payload->impulse_3);
	i += sizeof payload->impulse_3;

	memcpy(&buffer[i], &payload->battery, sizeof payload->battery);
	i += sizeof payload->battery;

	memcpy(&buffer[i], &payload->signal_quality, sizeof payload->signal_quality);
	i += sizeof payload->signal_quality;

	memcpy(&buffer[i], &payload->time_measure, sizeof payload->time_measure);
	i += sizeof payload->time_measure;

	*size = i;
	
	return 0;
}
//-------------------------------------------------------------------------------------------------
int Convert_Payload_to_Hex_Format(uint8_t* payload, uint16_t payload_size, uint8_t* payload_hex, uint16_t* payload_hex_size)
{
  uint16_t i;

  for (i = 0; i < payload_size; i++)
    sprintf((char*)payload_hex + i * 2, "%02x", payload[i]);
	*payload_hex_size	= payload_size*2;
	
  return true;
}
//-------------------------------------------------------------------------------------------------

