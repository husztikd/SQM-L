#include "Sleep.h"
#include <atmel_start.h>
#include <hpl_pm_base.h>
#include "Debug.h"
#include "Timer.h"
#include "I2c.h"
#include "SQM_Defines.h"
#include "Measure.h"
#include "Configuration.h"

#define CONFIG_SLEEPMGR_ENABLE

//-------------------------------------------------------------------------------------------------
void Sleep_Init(void)
{

}
//-------------------------------------------------------------------------------------------------
void Sleep(void)
{

	gpio_set_pin_level(FPWM_EN, false);
	gpio_set_pin_level(DONE, false);
	gpio_set_pin_level(FPWM_EN, false);	

	gpio_set_pin_level(PA22, true);	// SDA
	gpio_set_pin_level(PA23, true);	// SCL

	usart_async_disable(&USART_0);
	i2c_m_sync_disable(&I2C_0);

#ifdef USB_DEBUG
	usbdc_detach();
#endif

	gpio_set_pin_level(TP3, false);

	while(Get_Measure_Need_To_Send() == false)
	{
//		gpio_set_pin_level(TP3, false);	
		sleepmgr_sleep(3);
//		gpio_set_pin_level(TP3, true);			
//		delay_ms(1);
	}

	Set_Measure_Time(configuration->tm);
	Timer_Delay_ms(100);
	gpio_set_pin_level(TP3, true);			
	
	gpio_set_pin_level(FPWM_EN, true);	
	usart_async_enable(&USART_0);
	i2c_m_sync_enable(&I2C_0);

//	I2C_Init();
		
#ifdef USB_DEBUG
	usbdc_attach();
#endif
	
}
//-------------------------------------------------------------------------------------------------


