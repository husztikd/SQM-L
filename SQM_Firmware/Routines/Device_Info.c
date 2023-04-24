#include "Debug.h"
#include "Usb.h"
#include "Timer.h"
#include "SQM_Defines.h"
#include "Configuration.h"
#include "RN2483.h"

//-------------------------------------------------------------------------------------------------
void Device_Info(void)
{
	int delay = DEVICE_INFO_DELAY;
	
	if(Is_USB_Connected())
	{
		while(delay--)
		{
			Debug_Printf("%i\n", delay);	
			Timer_Delay_ms(1000);
		}
		Debug_Printf("*** SQM-L ***");	
		Debug_Printf("Device fw ver: %s", SOFTWARE_VERSION);
		Debug_Printf("LoRa DevEUI: %s", RN2483_Get_Deveui_Value());
		Debug_Printf("Battery Voltage: %i mV", RN2483_Get_Voltage_Value());
		Debug_Printf("TM: %u", configuration->tm);													
		Debug_Printf("*** END ***\n");			
	}
}
//-------------------------------------------------------------------------------------------------