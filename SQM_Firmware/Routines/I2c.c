#include "I2c.h"
#include <atmel_start.h>


struct io_descriptor *I2C_0_io;

//-------------------------------------------------------------------------------------------------
void I2C_Init(void)
{
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
  i2c_m_sync_set_slaveaddr(&I2C_0, 0x68, I2C_M_SEVEN);		
}
//-------------------------------------------------------------------------------------------------
uint8_t i2c_writeData(uint8_t slaveAddress, uint8_t *txBuffer, uint16_t size)
{
	uint32_t nmbrcharsWritten = 0;
	
	i2c_m_sync_set_slaveaddr(&I2C_0, slaveAddress, I2C_M_SEVEN);
	nmbrcharsWritten = io_write(I2C_0_io, (uint8_t *)txBuffer, size);
	if (nmbrcharsWritten == size)
		return 0;
	return 1;
}
//-------------------------------------------------------------------------------------------------
uint16_t i2c_readData(uint8_t slaveAddress, uint8_t *rxBuffer, uint16_t size)
{
	uint32_t charsReceived = 0;

	i2c_m_sync_set_slaveaddr(&I2C_0, slaveAddress, I2C_M_SEVEN);
	charsReceived = io_read(I2C_0_io, rxBuffer, size);
	return charsReceived;
}
//-------------------------------------------------------------------------------------------------