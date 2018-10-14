/*
 * Systick_cfg.h
 *
 *  Created on: 18/06/2014
 *      Author: Agustin
 */

#ifndef SYSTICK_CFG_H_
#define SYSTICK_CFG_H_

/********************************************************************************************************
*** INCLUDES
********************************************************************************************************/
void HandlerSystick500ms();
/*===========================================================================*
 * Exported Preprocessor #define X-MACROS
 *===========================================================================*/
#define TIMERS_MAX_VALUE sizeof(uint32_t)

#define TMR_TICK	1			//!< Período de Interrupción del Timer expresado en milisegundos

#define TABLA_TIMERS\
	/*
	 * 		Nombre				ID			TIPO			Valor	AutoRecarga		AutoArranque	Handler*/\
	TIMER("TIMER_2MSEC",	TIMER_1MSEC,	PERIODIC,		1, 		1,				1,				0)\
	TIMER("TIMER_10MSEC",	TIMER_10MSEC,	SINGLE_SHOT,	10, 	0,				0,				0)\
	TIMER("TIMER_20MSEC",   TIMER_20MSEC,	PERIODIC,		20,		20,				1,				0)\
	TIMER("TIMER_USER_DEF2",TIMER_500MSEC,  PERIODIC,		500,	500,			1,				HandlerSystick500ms)\
	TIMER("TIMER_USER_DEF3",TIMER_USER_DEF3,PERIODIC,	500,	500,			1,				0)\
	TIMER("TIMER_USER_DEF4",TIMER_USER_DEF4,SINGLE_SHOT,	0,		0,				0,				0)\
	TIMER("TIMER_USER_DEF5",TIMER_USER_DEF5,SINGLE_SHOT,	0,		0,				0,				0)\
/********************************************************************************************************
*** MODULE END
********************************************************************************************************/

#endif /* SYSTICK_CFG_H_ */
