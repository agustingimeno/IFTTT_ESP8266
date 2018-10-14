/*
 * esp8266.h
 *
 *  Created on: Sep 24, 2018
 *      Author: Agustin G. Gimeno (AGG)
 */
/*****************************************************************************************************//**
*
* @file		esp8266.h
* @brief
* @version	1.00
* @date		XX/XX/XXXX
* @author	Agustin Gonzalo Gimeno
*
*
*********************************************************************************************************
*** REVISION HISTORY
*
********************************************************************************************************/

#ifndef ESP8266_H_
#define ESP8266_H_

/********************************************************************************************************
*** MODULE
********************************************************************************************************/

/********************************************************************************************************
*** INCLUDES
********************************************************************************************************/

/********************************************************************************************************
*** DEFINES
********************************************************************************************************/

/********************************************************************************************************
*** MACROS
********************************************************************************************************/

/********************************************************************************************************
*** DATA TYPES
********************************************************************************************************/

/********************************************************************************************************
*** PUBLIC GLOBAL VARIABLES
********************************************************************************************************/

/********************************************************************************************************
*** PUBLIC FUNCTION PROTOTYPES
********************************************************************************************************/
void ESP_WifiConnect(void);
void ESP_TCPSend(char*, char*, char*);
bool_t ESP_Wifi_IsConnected(void);
void ESP_SM_Wifi(void);
void ESP_SM_TCP(void);

/********************************************************************************************************
*** MODULE END
********************************************************************************************************/
#endif /* ESP8266_H_ */

