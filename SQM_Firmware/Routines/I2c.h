#ifndef I2C_H_
#define I2C_H_

#include "Common.h"

void I2C_Init(void);
uint8_t i2c_writeData(uint8_t slaveAddress, uint8_t *txBuffer, uint16_t size);
uint16_t i2c_readData(uint8_t slaveAddress, uint8_t *rxBuffer, uint16_t size);

#endif /* I2C_H_ */