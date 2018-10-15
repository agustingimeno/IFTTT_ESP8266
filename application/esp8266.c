/*
 * esp8266.c
 *
 *  Created on: Sep 24, 2018
 *      Author: Agustin G. Gimeno (AGG)
 */


/****************************************************************************************************//**
*
* @file		esp8266.c
* @brief
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
#include "esp8266_cfg.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "My_Types.h"
#include "ringbuffer.h"
#include "uart.h"
/********************************************************************************************************
*** PRIVATE DEFINES
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE MACROS
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE DATA TYPES
********************************************************************************************************/
typedef enum {
	WIFI_DISCONNECTED,
	WIFI_WAIT_CONNECTED,
	WIFI_CONNECTED,
	TCP_WAIT_CONNECTED,
	TCP_CONNECTED,
	TCP_SEND_DATA_LENGTH,
	TCP_WAIT_SENDING_DATA_LENGTH,
	TCP_WRITING_DATA,
	TCP_SENDING_DATA,
}esp_states;
/********************************************************************************************************
*** PUBLIC GLOBAL VARIABLES
********************************************************************************************************/

/********************************************************************************************************
*** PRIVATE GLOBAL VARIABLES
********************************************************************************************************/
static esp_states esp_state;
static char recv_data[1024];
static char payload[1024];

static int multiplier = 0;

/********************************************************************************************************
*** PRIVATE FUNCTION PROTOTYPES
********************************************************************************************************/
void clear_payload(void);
void clear_timeout(void);
bool_t timeoutWaitingOnWifiConnect(void);
bool_t timeout(int timeout);
bool_t check_response(const char *cmp);
void clear_recv(void);

/********************************************************************************************************
*** CONFIGURATION ERRORS
********************************************************************************************************/

/*-----------------------------------------------------------------------------------------------------*/

/********************************************************************************************************
*** MODULE FUNCTIONS
********************************************************************************************************/
void ESP_WifiConnect(void){
	esp_state = WIFI_DISCONNECTED;
	char data[256];
	sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
	write_uart(data, strlen(data));
	esp_state = WIFI_WAIT_CONNECTED;
}

void ESP_TCPSend(char* host, char* port, char* in_payload){
	char data[256];
	sprintf(data, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", host, port);
	printf("\nTo Tx:%s \n",data);
	if(WIFI_CONNECTED <= esp_state){
		write_uart( data, strlen(data));
		esp_state = TCP_WAIT_CONNECTED;
		clear_payload();
		sprintf(payload, in_payload);
		printf(payload);
	}
}

bool_t ESP_Wifi_IsConnected(void){
	return (esp_state >= WIFI_CONNECTED);
}

void ESP_SM_Wifi(void){
	switch(esp_state)
	{
		case WIFI_DISCONNECTED:
			break;
		case WIFI_WAIT_CONNECTED:
			if (timeout(WAIT_WIFI_TO_CONNECT_MSEC)){
				esp_state = WIFI_DISCONNECTED;
				clear_recv();
				clear_timeout();
			}else if(check_response("OK")){
				esp_state = WIFI_CONNECTED;
				clear_recv();
				clear_timeout();
			}
			break;
		case WIFI_CONNECTED:
			break;
	}
}

void ESP_SM_TCP(void){
	char data[512];
	switch(esp_state)
	{
		case TCP_WAIT_CONNECTED:
			if (timeout(WAIT_TCP_TO_CONNECT_MSEC)){
				esp_state = WIFI_CONNECTED;
				clear_recv();
				clear_timeout();
			}else if(check_response("OK")){
				esp_state = TCP_CONNECTED;
				clear_recv();
				clear_timeout();
				sprintf(data, "AT+CIPSEND=%d\r\n", strlen(payload));
				write_uart(data, strlen(data));
			}
			break;
		case TCP_CONNECTED:
			if (timeout(WAIT_TCP_TO_CONNECT_MSEC)){
				//Failed
				esp_state = WIFI_CONNECTED;
				clear_recv();
				clear_timeout();
			}else if(check_response("OK")){
				esp_state = TCP_SENDING_DATA;
				clear_recv();
				clear_timeout();
				write_uart(payload, strlen(payload));
			}
		case TCP_SENDING_DATA:
			if (timeout(WAIT_TCP_TO_CONNECT_MSEC)){
				//Failed
				esp_state = TCP_CONNECTED;
				clear_recv();
				clear_timeout();
			}else if(check_response("SEND OK")){
				esp_state = TCP_CONNECTED;
				clear_recv();
				clear_timeout();
			}
			break;
	}
}

/********************************************************************************************************
*** INTERNAL MODULE FUNCTIONS
********************************************************************************************************/

bool_t timeoutWaitingOnWifiConnect(){
	static int multiplier = 0;
	multiplier++;

	if((BASE_TIME * multiplier) > WAIT_WIFI_TO_CONNECT_MSEC){
		return TRUE;
	}else{
		return FALSE;
	}
}

void clear_timeout(){
	multiplier = 0;
}

bool_t timeout(int timeout){
	multiplier++;
	if((BASE_TIME * multiplier) > timeout){
		return TRUE;
	}else {
		return FALSE;
	}
}

bool_t check_response(const char *cmp) {
	read_uart(recv_data);
	if(strstr(recv_data, cmp)){
		return TRUE;
	}else
	{
		return FALSE;
	}
}

void clear_recv(void){
	memset(recv_data, 0x00, 1024);
}

void clear_payload(){
	memset(payload, 0x00, 1024);
}

/*-----------------------------------------------------------------------------------------------------*/

