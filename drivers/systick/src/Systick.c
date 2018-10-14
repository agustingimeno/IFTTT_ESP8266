/****************************************************************************************************//**
*
* @file		Systick.c
* @brief	Módulo para manejo de Temporizaciones.
* @version	1.00
* @date		XX/XX/XXXX
* @author	Daniel J. López Amado (DJLA)
*
*
*********************************************************************************************************
*** REVISION HISTORY
*
********************************************************************************************************/


/********************************************************************************************************
*** INCLUDES
********************************************************************************************************/
#include "Systick.h"
#include "systick_private.h"
#include "LPC17xx.h"

/********************************************************************************************************
*** PRIVATE DEFINES
********************************************************************************************************/


/********************************************************************************************************
*** PRIVATE MACROS
********************************************************************************************************/
#define	STCTRL				SysTick->CTRL							/*!< SysTick Control and Status Register */
#define	STRELOAD			SysTick->LOAD							/*!< SysTick Reload Value Register       */
#define	STCURR				SysTick->VAL							/*!< SysTick Current Value Register      */
#define	STCALIB				SysTick->CALIB							/*!< SysTick Calibration Register        */

/*********************************************************************//**
 * Macro defines for System Timer Control and status (STCTRL) register
 **********************************************************************/
#define ST_CTRL_ENABLE		((uint32_t)(1<<0))
#define ST_CTRL_TICKINT		((uint32_t)(1<<1))
#define ST_CTRL_CLKSOURCE	((uint32_t)(1<<2))
#define ST_CTRL_COUNTFLAG	((uint32_t)(1<<16))

/********************************************************************************************************
*** PRIVATE DATA TYPES
********************************************************************************************************/


/********************************************************************************************************
*** PRIVATE TABLES
********************************************************************************************************/


/********************************************************************************************************
*** PUBLIC GLOBAL VARIABLES
********************************************************************************************************/


/********************************************************************************************************
*** PRIVATE GLOBAL VARIABLES
********************************************************************************************************/


/********************************************************************************************************
*** PRIVATE FUNCTION PROTOTYPES
********************************************************************************************************/


/********************************************************************************************************
*** CONFIGURATION ERRORS
********************************************************************************************************/


/*-----------------------------------------------------------------------------------------------------*/


/********************************************************************************************************
*** INTERNAL MODULE FUNCTIONS
********************************************************************************************************/
/***************************************************************************//**
 * @fn			Systick_Init()
 * @brief		Inicializa el Timer para interrumpir periodicamente
 *              segun la MACRO "TMR_TICK"
 * @param[in]	void
 * @return		void
 ******************************************************************************/
void timer_Systick_Init(void)
{
	// Para el cálculo del valor de recarga se dejan dos alternativas (Una de ellas se deja comentada)
	STRELOAD = (CCLK/1000)*TMR_TICK - 1;	// Carga la cuenta que define el tiempo entre interrupciones
	//STRELOAD = STCALIB;					// Carga la cuenta para que genere una interrupcion de 10ms, siempre que el clock sea de 100Mhz

	STCTRL |= ST_CTRL_CLKSOURCE;			// Selecciona la fuente de clock
	STCURR = 0;								// Resetea la cuenta actual (Pone a cero el contador)
	STCTRL |= ST_CTRL_TICKINT;				// Habilita la interrupción
	STCTRL |= ST_CTRL_ENABLE;				// Habilita el inicio de conteo
}

/***************************************************************************//**
 * @fn			SysTick_Handler()
 * @brief		Handler de Interrupción de Timer (System Tick Timer)
 *              El nombre de la Interrupción está definido por la CMSIS
 * @param[in]	void
 * @return		void
 ******************************************************************************/
void SysTick_Handler(void)
{
	static uint32_t	tick = 0;

	STCTRL &= ~ST_CTRL_COUNTFLAG;	// Limpia el flag de que el contador llego a cero

	timer_Core();

	if(++tick == _1msec)
	{
		tick = 0;
	}
}


/*-----------------------------------------------------------------------------------------------------*/
