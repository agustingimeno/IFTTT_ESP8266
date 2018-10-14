/*
 * uart.h
 *
 *  Created on: Aug 16, 2017
 *      Author: agustin
 */

#ifndef UART_API_UART_H_
#define UART_API_UART_H_

/********************************************************************************************************
*** INCLUDES
********************************************************************************************************/
#include "LPC17xx.h"
#include "My_Types.h"
#include "stdint.h"
#include "stdio.h"
#include "gpio.h"


/********************************************************************************************************
*** DEFINES
********************************************************************************************************/
#define IIR_RBR			0x02
#define IIR_THER 		0x01


/*********		******/
//#define Registro_de_Recepcion 	(*((__IO uint32_t *)UL))
//#define Registro_de_Transmicion	(*((__IO uint32_t *)UL))

/******** REGISTROS ***********/
#define UART0	(*((__IO uint32_t *)0x4000C000UL))
#define UART1	(*((__IO uint32_t *)0x40010000UL))
#define UART2	(*((__IO uint32_t *)0x40098000UL))
#define UART3	(*((__IO uint32_t *)0x4009C000UL))

/*----------RBR (DLAB =0)---------*/
#define U0RBR	(*((__IO uint32_t *)0x4000C000UL))
#define U2RBR 	(*((__IO uint32_t *)0x40098000UL))
#define U3RBR  	(*((__IO uint32_t *)0x4009C000UL))

/*----------THR (DLAB =0)---------*/
#define U0THR 	(*((__IO uint32_t *)0x4000C000UL))
#define U2THR 	(*((__IO uint32_t *)0x40098000UL))
#define U3THR 	(*((__IO uint32_t *)0x4009C000UL))

/*----------DLL (DLAB =1)---------*/
#define U0DLL   (*((__IO uint32_t *)0x4000C000UL))
#define U2DLL   (*((__IO uint32_t *)0x40098000UL))
#define U3DLL   (*((__IO uint32_t *)0x4009C000UL))

/*----------DLM (DLAB =1)---------*/
#define U0DLM   (*((__IO uint32_t *)0x4000C004UL))
#define U2DLM   (*((__IO uint32_t *)0x40098004UL))
#define U3DLM   (*((__IO uint32_t *)0x4009C004UL))

/*----------IER (DLAB =0)---------*/
#define U0IER   (*((__IO uint32_t *)0x4000C004UL))
#define U2IER   (*((__IO uint32_t *)0x40098004UL))
#define U3IER   (*((__IO uint32_t *)0x4009C004UL))

/*----------IIR ------------------*/
#define U0IIR   (*((__IO uint32_t *)0x4000C008UL))
#define U2IIR   (*((__IO uint32_t *)0x40098008UL))
#define U3IIR   (*((__IO uint32_t *)0x4009C008UL))

/*----------FCR ------------------*/
#define U0FCR   (*((__IO uint32_t *)0x4000C008UL))
#define U2FCR   (*((__IO uint32_t *)0x40098008UL))
#define U3FCR   (*((__IO uint32_t *)0x4009C008UL))

/*----------LCR ------------------*/
#define U0LCR   (*((__IO uint32_t *)0x4000C00CUL))
#define U2LCR   (*((__IO uint32_t *)0x4009800CUL))
#define U3LCR   (*((__IO uint32_t *)0x4009C00CUL))

/*----------LSR ------------------*/
#define U0LSR   (*((__IO uint32_t *)0x4000C014UL))
#define U2LSR   (*((__IO uint32_t *)0x40098014UL))
#define U3LSR   (*((__IO uint32_t *)0x4009C014UL))

/*----------SCR ------------------*/
#define U0SCR   (*((__IO uint32_t *)0x4000C01CUL))
#define U2SCR   (*((__IO uint32_t *)0x4009801CUL))
#define U3SCR   (*((__IO uint32_t *)0x4009C01CUL))

/*----------ACR ------------------*/
#define U0ACR   (*((__IO uint32_t *)0x4000C020UL))
#define U2ACR   (*((__IO uint32_t *)0x40098020UL))
#define U3ACR   (*((__IO uint32_t *)0x4009C020UL))

/*----------ICR ------------------*/
#define U0ICR   (*((__IO uint32_t *)0x4000C024UL))
#define U2ICR   (*((__IO uint32_t *)0x40098024UL))
#define U3ICR   (*((__IO uint32_t *)0x4009C024UL))

/*----------FDR ------------------*/
#define U0FDR   (*((__IO uint32_t *)0x4000C028UL))
#define U2FDR   (*((__IO uint32_t *)0x40098028UL))
#define U3FDR   (*((__IO uint32_t *)0x4009C028UL))

/*----------TER ------------------*/
#define U0TER   (*((__IO uint32_t *)0x4000C030UL))
#define U2TER   (*((__IO uint32_t *)0x40098030UL))
#define U3TER   (*((__IO uint32_t *)0x4009C030UL))

/********************************************************************************************************
*** DATA TYPES
********************************************************************************************************/


/********************************************************************************************************
*** MACROS
********************************************************************************************************/
/*----------IER (DLAB =0)---------*/
#define IER_RBR			( 0x01 << 0)			//IERRBRInterruptEnable
#define IER_THR			( 0x01 << 1)			//IERTHREInterruptEnable
#define IER_RX			( 0x01 << 2)			//IERRXLineStatusInterruptEnable
#define IER_ABEOIntEn	( 0x01 << 8)			//IERABEOIntEn
#define IER_ABTOIntEn	( 0x01 << 9)			//IERABTOIntEn

/*----------IIR ------------------*/
#define IntStatusU0	(U0IIR | (0 << 0))			//Interrupt status.
#define IntStatusU2	(U2IIR | (0 << 0))
#define IntStatusU3	(U3IIR | (0 << 0))

#define IIR_IntIdU(n)		((n >> 1) & 0x07)			//Interrupt identification
														//(U0IIR >> n)
														// leer 3 bits (2,3,4)

/*----------FCR ------------------*/

/*----------LCR ------------------*/
#define LCR_WLS5 	(0x00 << 0)					// 00 5-bit character length
#define LCR_WLS6 	(0x01 << 0)					// 01 6-bit character length
#define LCR_WLS7 	(0x02 << 0)					// 10 7-bit character length
#define LCR_WLS8	(0x03 << 0)					// 11 8-bit character length
#define LCR_SBS1	(0x00 << 2)					// 1 stop bit
#define LCR_SBS2	(0x01 << 2)					// 2 stop bits (1.5 if UnLCR[1:0]=00)
#define LCR_PE		(0x01 << 3)					//Enable parity generation and checking
#define LCR_PS00	(0x00 << 4)					// Odd parity.
#define LCR_PS01 	(0x01 << 4)					// Even Parity.
#define LCR_PS10 	(0x10 << 4)					// Forced "1" stick parity.
#define LCR_PS11 	(0x11 << 4)					// Forced "0" stick parity.
#define LCR_BC		(0x01 << 6)					//Enable break transmission
#define LCR_DL		(0x01 << 7)					//Access Bit (DLAB). Enable access to Divisor Latches

/*----------LSR ------------------*/

/*----------SCR ------------------*/

/*----------ACR ------------------*/

/*----------ICR ------------------*/

/*----------FDR ------------------*/
#define DIVADDVAL   (0x0F << 0)
#define MULVAL		(0x0F << 4)


/********************************************************************************************************
*** DATA TYPES
********************************************************************************************************/

/********************************************************************************************************
*** PUBLIC GLOBAL VARIABLES
********************************************************************************************************/

/********************************************************************************************************
*** PUBLIC FUNCTION PROTOTYPES
********************************************************************************************************/
void UART_Init();
void UART_PushTx(unsigned char dato);
char UART_PopTx(void);
void UART_PushRx(unsigned char dato);
char UART_PopRx(void);
void UART_RxClean(void);
void UART_IRQHandler(void);

void UART1_Init ( uint32_t );

void UART1_PushRX( uint8_t dato );
int UART1_PopRX( char * data );
void UART1_PushTX( uint8_t dato );
int16_t UART1_PopTX( void );

/********************************************************************************************************
*** MODULE END
********************************************************************************************************/


#endif /* UART_API_UART_H_ */
