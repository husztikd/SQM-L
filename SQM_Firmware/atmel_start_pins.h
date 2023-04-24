/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define B_LO GPIO(GPIO_PORTA, 6)
#define PWR_OK GPIO(GPIO_PORTA, 7)
#define FPWM_EN GPIO(GPIO_PORTA, 8)
#define IMP_2 GPIO(GPIO_PORTA, 9)
#define IMP GPIO(GPIO_PORTA, 10)
#define IMP_1 GPIO(GPIO_PORTA, 11)
#define TP3 GPIO(GPIO_PORTA, 15)
#define CS_FLASH GPIO(GPIO_PORTA, 18)
#define WAKE GPIO(GPIO_PORTA, 20)
#define DONE GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define RI_N GPIO(GPIO_PORTB, 2)
#define LORA_RESET GPIO(GPIO_PORTB, 9)
#define PB22 GPIO(GPIO_PORTB, 22)
#define PB23 GPIO(GPIO_PORTB, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
