/*****************************************************************************************************//**
*
* @file		timer_systick.c
* @brief	Modulo para manejo de Timers
*
*
* @version	1.00
* @date		XX/XX/XXXX
* @author	Agustin G. Gimeno (AGG)
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
#include "stdio.h"


/********************************************************************************************************
*** PRIVATE DEFINES
********************************************************************************************************/


/********************************************************************************************************
*** PRIVATE MACROS
********************************************************************************************************/


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
volatile static Timer_Systick_Type timers[TIMERS_MAX_NUM] = {
	#undef TIMER
	#define TIMER(nombre, id, tipo, valor, autorecarga, autoarranque, handler) {nombre, id, tipo, valor, autorecarga, autoarranque, handler, 0},
		TABLA_TIMERS
	#undef TIMER
};

/********************************************************************************************************
*** PRIVATE FUNCTION PROTOTYPES
********************************************************************************************************/
static bool_t timer_Check_Validity_Start(timer_id id, tiempo valor, timer_types tipo, bool_t* error_code);


/********************************************************************************************************
*** CONFIGURATION ERRORS
********************************************************************************************************/


/*-----------------------------------------------------------------------------------------------------*/


/********************************************************************************************************
*** PUBLIC FUNCTIONS
********************************************************************************************************/

/********************************************************************************************************
 * @fn	TIMER_Systick_Init
 * @brief	Inicializacion del Modulo Systick Timer.
 *
 * @param[in] 	ninguno
 * @return		nada
********************************************************************************************************/
void TIMER_Systick_Init(void)
{
	timer_Systick_Init();
}

/********************************************************************************************************
 * @fn	TIMER_Start
 * @brief	Cuenta la cantidad de tiempo espeficada en valor (normalmente miliSegundos) y provee
 * 			la información de que se termino de contar a través de un flag que puede se encuestado
 * 			en cualquier momento por la funcion TIMER_Get_Event o llama dentro de la misma
 * 			interrupcion al puntero a funcion que se pasa como argumento (en el caso de que sea
 * 			NULL se sigue teniendo el flag).
 *
 * @param[in]	timer_id id 			identificador del timer.
 * @param[in]	tiempo valor 			normalmente espeficado en mSec depende de .
 * @param[in]	timer_types tipo 		Periodic se autorecarga con el valor especificado
 * 										Single_Shot solo genera una vez el flag, el valor debe
 * 										ser recargado manualmente para voler a utilizar el timer.
 * @param[in]	void (*handler)(void) 	puntero a la funcion a ser llamada cuando se vence el timer.
 * 										IMPORTANTE!: esta funcion debe ser breve ya que forma parte
 * 										de la rutina de interrupcion del Systick.
 *
 * @return		TRUE					Se pudo arrancar el timer.
 * 				FALSE					Hubo un problema.
********************************************************************************************************/
bool_t TIMER_Start(timer_id id, tiempo valor, timer_types tipo ,void (*handler)(void))
{
	bool_t	error_code;

	if(TRUE == timer_Check_Validity_Start(id, valor, tipo, &error_code))
	{
		timers[id].tipo = tipo;
		timers[id].valor = valor;
		timers[id].flag_evento = FALSE;
		if(NULL != handler)
		{
			timers[id].handler = handler;
		}
		if(PERIODIC==tipo)
		{
			timers[id].autorecarga = valor;
		}
	}
	return error_code;
}


/********************************************************************************************************
 * @fn	TIMER_Stop
 * @brief	Para el timer, borra la cuenta, el puntero y el flag.
 *
 * @param[in]	time_id id	identificador del timer.
 *
 * @return		TRUE		si el resultado fue satisfactorio.
 * 				FALSE		si ocurrio un problema.
********************************************************************************************************/
bool_t TIMER_Stop(timer_id id)
{
	bool_t	error_code = TRUE;
	/*chequeo de id valido*/
	if((TIMERS_MAX_NUM > id) && (id >0))
	{
		timers[id].flag_evento = 0;
		timers[id].handler = NULL;
		timers[id].valor = 0;
	}else
	{
		error_code = FALSE;
	}
	return error_code;
}

/********************************************************************************************************
 * @fn	TIMER_Get_Event
 * @brief	Funcion para encuestar y saber si se vencio el timer. Equivalente a preguntar por un
 * 			flag.
 * 			IMPORTANTE!: en caso de que el timer expiro, el flag es borrado.
 *
 * @param[in] timer_id	id 	identificador del timer que se desea encuestar.
 *
 * @return		TRUE		si el timer expiro
 * 				FALSE		si el timer no expiro desde la ultima encuestra.
********************************************************************************************************/
bool_t TIMER_Get_Event(timer_id id)
{

	bool_t retval = FALSE;
	if(TIMERS_MAX_NUM > id)
	{
		if(TRUE == timers[id].flag_evento)
		{
			retval = TRUE;
			timers[id].flag_evento = FALSE;
		}
	}
	return retval;
}

/********************************************************************************************************
*** PRIVATE FUNCTIONS
********************************************************************************************************/

/********************************************************************************************************
 * @fn		timer_Core
 * @brief	Es la funcion central y es privada al modulo, se encarga de ir descontando los timers
 * 			incializados, llamados a los punteros a funcion provistos y marcando los flags de los timers
 * 			ya vencidos. Es llamada en la interrupcion del modulo de Systick.
 * 			En el caso de que el timer tenga autorecarga, vuelve a cargar automaticamente el timer
 * 			con el valor original provisto en la funcion Start o sacado desde el archivo de configuracion.
 *
 * @param[in]	void
 * @return		void
********************************************************************************************************/
void timer_Core()
{
	uint32_t i = 0;
	for(i = 0; i < TIMERS_MAX_NUM; i++)
	{
		if(0 < timers[i].valor)
		{
			timers[i].valor--;
			if(0 == timers[i].valor)
			{
				timers[i].flag_evento = TRUE;
				if(NULL != timers[i].handler)
				{
					(*timers[i].handler)();
				}
				if(PERIODIC == timers[i].tipo)
				{
					timers[i].valor = timers[i].autorecarga;
				}
			}
		}
	}
}

/********************************************************************************************************
 * @fn	timer_Check_Validity_Start
 * @brief Es una funcion privada que sirve para validad los parametros pasados a la funcion Start.
 *
 * @param[in]
 * @return
********************************************************************************************************/
static bool_t timer_Check_Validity_Start(timer_id id, tiempo valor, timer_types tipo, bool_t* error_code)
{
	bool_t retval = TRUE;
	/*chequeo de id valido*/
	if((TIMERS_MAX_NUM > id) && (id >0))
	{
		/*Estaba en uso? Debe primero detenerlo!*/
		if(0 != timers[id].valor)
		{
			*error_code = TRUE;
		}else
		{
			if((TIMERS_MAX_VALUE < valor) || (0 > valor))
			{
				*error_code = TRUE;
			}else
			{
				if((0>tipo)||(TIMER_TYPES_NUM_MAX<=tipo))
				{
					*error_code = TRUE;
				}
			}
		}
	}else
	{
		*error_code = TRUE;
	}

	if(*error_code == TRUE)
	{
		retval = FALSE;
	}

	return retval;
}

/*-----------------------------------------------------------------------------------------------------*/



