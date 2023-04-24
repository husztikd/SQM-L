#include "Usb.h"
#include <atmel_start.h>
#include "Timer.h"
#include "SQM_Defines.h"

bool usb_connected = false;

//-------------------------------------------------------------------------------------------------
void USB_Init(void)
{
	TIMER_T	timeout_connect;

#ifndef USB_DEBUG
	return;
#endif

	timeout_connect = SetTimeout(USB_CONNNECT_TIMEOUT);

	while(!AfterTime(timeout_connect))
	{
		if(cdcdf_acm_is_enabled())
		{
			usb_connected = true;
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------
bool Is_USB_Connected(void)
{
	return usb_connected;
}
//-------------------------------------------------------------------------------------------------
