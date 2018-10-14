/*
 ===============================================================================
 Name        : CP_Procesar_Trama.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

#include "boardinit.h"

#include "Systick.h"

#include "esp8266.h"
#include "ifttt.h"

int main(void)
{
	int flag = 0;
	BoardInit();

	printf("ESP8266 IFTTT R2051\n");
	ESP_WifiConnect();
	while (1)
	{
		if(0 == flag && ESP_Wifi_IsConnected()){
			flag = 1;
			IFTTT_write(LOG_TEMPERATURE, 10, 100, 1000);
		}
		if(TIMER_Get_Event(TIMER_USER_DEF3)){
			//Feed the State Machines.
			ESP_SM_Wifi();
			ESP_SM_TCP();
		}
	}
	return 0;
}

void HandlerSystick500ms()
{

}

