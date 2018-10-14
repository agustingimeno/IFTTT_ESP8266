/*
 * ringbuffer.c
 *
 *  Created on: Sep 23, 2018
 *      Author: agustin
 */
#define MAX_DATOS	1024*5

#include "stdint.h"
#include "uart.h"
#include "LPC17xx.h"

int16_t write_uart (const void * datos , uint16_t cant )
{
	uint16_t	i;
	LPC_UART1->IER = 0x00000000;
	if ( cant > MAX_DATOS )
		return -1;

	for ( i = 0 ; i < cant ; i++ )
	{
		UART1_PushTX( * ( ( uint8_t * ) ( datos + i ) ) );
	}
	LPC_UART1->IER = 0x00000003;
	return 0 ;
}


