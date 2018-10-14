/*
 * ring_buffer.h
 *
 *  Created on: Apr 3, 2016
 *      Author: agustin
 */

#ifndef API_RINGBUFFER_H_
#define API_RINGBUFFER_H_

#include "stdint.h"
/********************************************************************************************************
*** PUBLIC FUNCTION PROTOTYPES
********************************************************************************************************/
#define MAX_DATOS	1024*5

int16_t write_uart(const void * datos , uint16_t cant );


#endif /* API_RINGBUFFER_H_ */
