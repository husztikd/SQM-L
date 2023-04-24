#ifndef SPM_DEFINES_H_
#define SPM_DEFINES_H_


//*************************************************************************************************
// Device Configuration
//*************************************************************************************************

// Version
#define DEVICE_VERSION "SQM-N"
#define HARDWARE_VERSION "1.2.1"
#define SOFTWARE_VERSION "1.0.0"
#define SOFTWARE_VERSION_NUM 100

// Lora
#define APPEUI "BE010000000008A2"
#define APPKEY "7D2BF734A253CF2449A88EA655356C4B"


// Impulse
#define IMPULSE_SENDING_TIME 10					// time between two packet in min


// USB
#define USB_DEBUG
#define USB_CONNNECT_TIMEOUT 10000				// in ms
#define DEVICE_INFO_DELAY 30							// in sec


//*************************************************************************************************
// Board Configuration
//*************************************************************************************************
// Buffer
#define DEBUG_BUFFER 256
#define UART_BUFFER 2048
#define PAYLOAD_BUFFER 512

// Impulse
#define IMPULSE_BLOCK_TIME 100		// in 30 ms
//#define IMPULSE_COUNTER_CLEAR_AT_READ

// Lora
#define RN2483_Reset_Pin_Low() gpio_set_pin_level(LORA_RESET, true)
#define RN2483_Reset_Pin_High() gpio_set_pin_level(LORA_RESET, false)


#endif /* SPM_DEFINES_H_ */