#ifndef UART_H_
#define UART_H_

#include "Common.h"

	void Uart_Init(void);
	uint16_t Uart_Rx_count(void);
	int Uart_Write_Bytes( uint8_t* data, uint16_t size);
	int Uart_Write_String(char* str);
	int Get_Uart_Byte(uint8_t* byte);
	void Uart_Break(void);
	void Uart_Disable(void);
#endif /* UART_H_ */