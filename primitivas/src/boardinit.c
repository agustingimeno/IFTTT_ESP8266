/*
 * boardinit.c
 *
 *  Created on: Aug 16, 2017
 *      Author: agustin
 */
#include "system_LPC17xx.h"
#include "gpio.h"
#include "uart.h"
#include "Systick.h"

void ESP8266_Init(void);

/***************************************************************************//**
 * @fn			BoardInit()
 * @brief		Configura todos los perifericos para trabajar con la placa.
 * 				Arranca el PLL del uC.
 *
 * @param[in]	None
 *
 * @return 		None
 ******************************************************************************/
void BoardInit(void)
{
	SystemInit();
	TIMER_Systick_Init();
	UART1_Init(115200);

	ESP8266_Init();
}

void ESP8266_Init(void){
	GPIO_InitPinAsGPIO(PORT_0, PIN_17, DIR_OUT, RES_NONE, NOT_OPEN_DRAIN);
	GPIO_SETpIN(PORT_0, PIN_17, ACTIVE_HIGH);
}
