#include "Configuration.h"
#include "SQM_Defines.h"

CONFIGURATION config;
//CONFIGURATION configuration;

const CONFIGURATION* const configuration = &config;

void Configuration_Init(void);
void Configuration_Load_From_Define(void);

//-------------------------------------------------------------------------------------------------
void Configuration_Init(void)
{
	Configuration_Load_From_Define();
}
//-------------------------------------------------------------------------------------------------
void Configuration_Load_From_Define(void)
{
	config.tm = IMPULSE_SENDING_TIME;
//	config.alpha = 0; 
//	config.p_min = 0;
//	config.p_max = 0;	
//	config.p_alert_min = 0;
//	config.p_alert_max = 0;
//	config.delta = 0;
	config.control = 0;
}
//-------------------------------------------------------------------------------------------------