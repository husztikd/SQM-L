#include "debug.h"
#include <atmel_start.h>
#include "SQM_Defines.h"

void vprint(const char *fmt, va_list argp);

//-------------------------------------------------------------------------------------------------
int debug_write_bytes(uint8_t *data, uint32_t size)
{

	uint8_t buffer[DEBUG_BUFFER] = {0};
	if(!size)
		return -1;
	if(size > sizeof(buffer))
		size = sizeof(buffer);
	memcpy(buffer, data, size);
	
#ifdef USB_DEBUG
	cdcdf_acm_write(buffer, size);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------------------
int debug_write_string(char *str)
{
	char buffer[DEBUG_BUFFER] = {0};

	size_t size = strlen(str);

	if(!size)
		return -1;
	if(size > (sizeof(buffer)-2))
		size = sizeof(buffer)-2;
	strncpy(buffer, str, size);
	strcat(buffer,"\r\n");
	
#ifdef USB_DEBUG
	cdcdf_acm_write((void*)buffer, strlen(buffer));
#endif

	return 0;
}
//-------------------------------------------------------------------------------------------------
void Debug_Printf(const char *fmt, ...)
{
  va_list argp;
  va_start(argp, fmt);
  vprint(fmt, argp);
  va_end(argp);
}
//-------------------------------------------------------------------------------------------------
void vprint(const char *fmt, va_list argp)
{
  char string[DEBUG_BUFFER] = {0};
  if(0 < vsprintf(string,fmt,argp)) // build string
  {
		debug_write_string(string);
  }
}

//-------------------------------------------------------------------------------------------------