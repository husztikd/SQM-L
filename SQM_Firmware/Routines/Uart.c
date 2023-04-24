#include "Uart.h"
#include <atmel_start.h>
#include "Circular_Buffer.h"
#include "SQM_Defines.h"
#include "Timer.h"

#define UART_RX_BUFFER_SIZE 512

struct io_descriptor *usart0_io;
struct usart_async_status USART_0_status;

volatile bool uart_tx_ready;

uint8_t bytes[UART_BUFFER];
uint8_t uart_rx_buffer[UART_BUFFER];
uint8_t uart_tx_buffer[UART_BUFFER];
CIRCULAR_BUFFER cb_rx_buffer;
CIRCULAR_BUFFER *p_cb_rx_buffer = &cb_rx_buffer;

void tx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	uart_tx_ready = true;
}

void rx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
}
//-------------------------------------------------------------------------------------------------
void Uart_Init(void)
{
	uart_tx_ready = true;

	Circular_Buffer_init(p_cb_rx_buffer, uart_rx_buffer, UART_BUFFER);	

	usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_USART_0);
	usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb_USART_0);	

	usart_async_get_io_descriptor(&USART_0, &usart0_io);

	usart_async_enable(&USART_0);
}
//-------------------------------------------------------------------------------------------------
uint16_t Uart_Rx_count(void)
{
	usart_async_get_status(&USART_0, &USART_0_status);
	return USART_0_status.rxcnt;
}
//-------------------------------------------------------------------------------------------------
int Uart_Write_Bytes(uint8_t* data, uint16_t size)
{
	if(size == 0 || size >= UART_BUFFER-2)
	return 1;
	
	memcpy(uart_tx_buffer, data, size);

	uart_tx_ready = false;
	int ret = io_write(usart0_io, (void*)uart_tx_buffer, size);

	while(uart_tx_ready != true);
	
	return ret;
}
//-------------------------------------------------------------------------------------------------
int Uart_Write_String(char* str)
{
	if(strlen(str) == 0 || strlen(str)>= UART_BUFFER-2)
		return 1;

	snprintf((char*)uart_tx_buffer, UART_BUFFER-2, "%s", str);
	strcat((char*)uart_tx_buffer,"\r");

	uart_tx_ready = false;
	int ret = io_write(usart0_io, (void*)uart_tx_buffer, strlen((char*)uart_tx_buffer));

	while(uart_tx_ready != true);
	
	return ret;
}
//-------------------------------------------------------------------------------------------------	

int Get_Uart_Byte(uint8_t* byte)
{
	if(Uart_Rx_count())
		return io_read(usart0_io, byte, 1);
	return 0;
}
//-------------------------------------------------------------------------------------------------
void Uart_Break(void)
{
	usart_async_disable(&USART_0);
	gpio_set_pin_function(PB22, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_direction(PB22, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PB22, false);
	Timer_Delay_ms(100);
	gpio_set_pin_level(PB22, true);
	gpio_set_pin_function(PB22, PINMUX_PB22D_SERCOM5_PAD2);
	usart_async_enable(&USART_0);
}
//-------------------------------------------------------------------------------------------------
