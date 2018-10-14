/*
 * infotronic.h
 *
 *  Created on: Jun 5, 2015
 *      Author: tux
 */
#ifndef OTHER_INC_INFOTRONIC_H_
#define OTHER_INC_INFOTRONIC_H_

#include "gpio.h"
//Identificación de los puertos de expansion:

// PLACA EXPANSION 2
#define DIG1_PORT	PORT_2
#define DIG2_PORT	PORT_1
#define DIG3_PORT	PORT_4
#define DIG4_PORT	PORT_1
#define DIG5_PORT	PORT_1
#define DIG6_PORT	PORT_0

#define DIG1_PIN	7
#define DIG2_PIN	29
#define DIG3_PIN	28
#define DIG4_PIN	23
#define DIG5_PIN	20
#define DIG6_PIN	19

#define seg_a_port  	PORT_3
#define seg_b_port		PORT_1
#define seg_c_port		PORT_1
#define seg_d_port		PORT_1
#define seg_e_port		PORT_1
#define seg_f_port		PORT_3
#define seg_g_port		PORT_0
#define seg_dp_port		PORT_1

#define seg_a_pin		26
#define seg_b_pin		18
#define seg_c_pin		21
#define seg_d_pin		24
#define seg_e_pin		27
#define seg_f_pin		25
#define seg_g_pin		20
#define seg_dp_pin		19

//#define EXPANSION26		PORT1,31
//#define EXPANSION27		PORT1,31
#define CANT_DIGITOS 6

#endif /* OTHER_INC_INFOTRONIC_H_ */
