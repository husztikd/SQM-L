#include <atmel_start.h>
#include "Common.h"
#include "Communication.h"
#include "Sleep.h"
#include "Debug.h"
//#include "I2c.h"
#include "Timer.h"
#include "Measure.h"
#include "Uart.h"
#include "Impulse.h"
#include "Watchdog.h"
#include "Device_Info.h"
#include "SQM_Defines.h"
#include "Configuration.h"
#include "Usb.h"
#include "Lora.h"

int main(void)
{
	atmel_start_init();
	gpio_set_pin_level(TP3, true);
	delay_ms(1000);

	Configuration_Init();		
//	I2C_Init();
	Uart_Init();
	Timer_Init();
	Sleep_Init();
	Impulse_Init();
	Measure_Init();
	USB_Init();	
	Lora_Init();
	Device_Info();
	

	while (1)
	{
		Measure();
		Watchdog_Reset();
		Communication();
		Sleep();
	}
}
