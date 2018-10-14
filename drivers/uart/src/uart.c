/*
 * uart.c
 *
 *  Created on: Aug 16, 2017
 *      Author: agustin
 */

/********************************************************************************************************
*** INCLUDES
********************************************************************************************************/

#include "ringbuffer.h"
#include "uart.h"
#include "My_Types.h"
#include "nvic.h"
#include "LPC17xx.h"


/********************************************************************************************************
*** PRIVATE DEFINES
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE MACROS
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE DATA TYPES
********************************************************************************************************/
volatile static unsigned char bufferTx=0;

/********************************************************************************************************
*** PRIVATE TABLES
********************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
volatile static uint8_t UART1_BufferRx[MAX_DATOS];
volatile static uint8_t UART1_BufferTx[MAX_DATOS];
volatile static uint16_t UART1_inRX,UART1_outRX,UART1_inTX,UART1_outTX;
volatile static int16_t UART1_flagTx;

/********************************************************************************************************
*** PRIVATE GLOBAL VARIABLES
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE FUNCTION PROTOTYPES
********************************************************************************************************/

/********************************************************************************************************
*** CONFIGURATION ERRORS
********************************************************************************************************/

/********************************************************************************************************
*** PUBLIC FUNCTIONS
********************************************************************************************************/
void UART_Init()
{
	// Implementar esta funcion para UART0 - 115200 - 8, N, 1

	SC_PCONP |= (0x01 << 3);			//Enciendo la UART0
	//pclk = 12.5 MHz
	SC_PCLKSEL0 |= (0x03 << 6);

	GPIO_SetPinFunc( PORT_0, PIN_2, FIRST_ALT_FUNC);
	GPIO_SetPinFunc( PORT_0, PIN_3, FIRST_ALT_FUNC);

	//U0LSR |= LCR_WLS8;
	//U0LSR |= LCR_SBS1;
	//U0LSR |= LCR_DL;
	U0LCR = 0x83;

	//9600 baudrate

	//Fdiv  = (pclk / 16) / baudrate;
	U0DLM = 0;
	U0DLL = 73;

	U0FDR |= ( 1 << 0);
	U0FDR |= ( 9 << 4);

	U0LCR &= ~LCR_DL;
	//U0FCR = 0x00;

	NVIC_EnableIRQ( UART0_IRQn);
	U0IER |= IER_RBR;
	U0IER |= IER_THR;

}

void UART1_Init ( uint32_t baudios )
{
	//!< 1.- Registro PCONP - bit 3 en 1 prende la UART:
	SC_PCONP |= 0x01<<3;

	//!< 2.- Registro PCLKSEL0 - bits 6 y 7 en 0 seleccionan que el clk de la UART0 sea CCLK/4:
	SC_PCLKSEL0 &= ~(0x03<<6);			//!< con un CCLOK=100Mhz, nos queda PCLOCK=25Mhz

	//!< 3.- Registro U1LCR - transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1:
	LPC_UART1->LCR = 0x00000083;

	//!< 4.- Registros U11DLL y U1DLM
	baudios =  25000000 / ( 16 * baudios );
	LPC_UART1->DLM = (baudios >> 8 ) & 0x000000ff;
	LPC_UART1->DLL = baudios & 0x000000ff;

	//!< 5.- Registros PINSEL0 y PINSEL1 - habilitan las funciones especiales de los pines:
	//!< TX1 : P0[15]	-> PINSEL0: 30:31
	GPIO_SetPinFunc( PORT_0 , 15, FIRST_ALT_FUNC );
	//!< RX0 : P0[16]	-> PINSEL0: 01:00
	GPIO_SetPinFunc( PORT_0 , 16 , FIRST_ALT_FUNC );

	//!< 6.- Registro U1LCR, pongo DLAB en 0:
	LPC_UART1->LCR = 0x00000003;

	//!< 7. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
	NVIC_EnableIRQ(UART1_IRQn);
	LPC_UART1->IER = 0x00000003;

}


/**
	\fn  void UART1_IRQHandler (void)
	\brief Interrupcion de UART1
 	\author Ing. Marcelo Trujillo
 	\date 31 de ago. de 2016
 	\param void
	\return void
*/
void UART1_IRQHandler (void)
{
	uint8_t iir, aux;
	int16_t datoTx;

	do
	{
		//IIR es reset por HW, una vez que lo lei se resetea.
		iir = LPC_UART1->IIR;

		if ( iir & 0x04 ) 							//Data ready
		{
			aux = LPC_UART1->RBR;
			UART1_PushRX ( aux );   	//guardo en buffer e incremento índice									//garantizo el buffer circular
		}

		if ( iir & 0x02 ) //THRE
		{
			datoTx = UART1_PopTX();
			if ( datoTx != -1 )
				LPC_UART1->THR = (uint8_t) datoTx;
			else
				UART1_flagTx = 0;
		}
	}
	while( ! ( iir & 0x01 ) ); /* me fijo si cuando entre a la ISR habia otra
						     	int. pendiente de atencion: b0=1 (ocurre unicamente si dentro del mismo
								espacio temporal lleguan dos interrupciones a la vez) */
}





/**
	\fn  void UART1_PushRX( uint8_t dato )
	\brief pone un Byte en el buffer de recepcion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void UART1_PushRX( uint8_t dato )
{
	UART1_BufferRx[ UART1_inRX ] = dato;
	UART1_inRX ++;
	UART1_inRX %= MAX_DATOS;
}

/**
	\fn  int16_t UART1_PopRX( void )
	\brief saca un Byte en el buffer de recepcion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int UART1_PopRX( char * data )
{
	int salida = -1;

	if ( UART1_inRX != UART1_outRX )
	{
		*data = (char)UART1_BufferRx[ UART1_outRX ] ;
		salida = 1;
		UART1_outRX ++;
		UART1_outRX %= MAX_DATOS;
	}
	return salida;
}
/**
	\fn  void UART1_PushTX( uint8_t dato )
	\brief pone un Byte en el buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void UART1_PushTX( uint8_t dato )
{
	if ( !UART1_flagTx )
	{
		UART1_flagTx = 1;
		LPC_UART1->THR = dato;
		return;
	}

	UART1_BufferTx[ UART1_inTX ] = dato;
	UART1_inTX ++;
	UART1_inTX %= MAX_DATOS;
}

/**
	\fn int16_t UART1_PopTX( void )
	\brief saca un Byte en el buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int16_t UART1_PopTX( void )
{
	int16_t salida = -1;

	if ( UART1_inTX != UART1_outTX )
	{
		salida = UART1_BufferTx[ UART1_outTX ] ;

		UART1_outTX ++;
		UART1_outTX %= MAX_DATOS;
	}
	return salida;
}
