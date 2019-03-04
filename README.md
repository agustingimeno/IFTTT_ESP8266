# IFTTT_ESP8266
Embedded C project based on NXP LPC1769. Using an ESP8266 to connect to IFTTT webhook and send data.


## Introduction

This project is all written in C language and based on the NXP LPC1769 using an ESP8266 connected to it through the UART1 of the MCU. Using the WIFI capabilities of the ESP8266 the idea is to send data to the cloud using IFTTT and the Webhook Service tied to a google spreadsheet, though it can be tied to any other service using the webhook.
This project is an example of sending data to the cloud just once, though the ifttt code and the esp8266 code support sending data periodically. 
The esp8266 code is not a full driver, it just supports connection to a WiFi Network and supports only one TCP client connection. 
The intention of all this is just  educational, so use it at your own risk.

## Main Architecture

The flow of the data is as follows:
  
![Alt text](/docs/images/Flow.jpg?raw=true "Flow")  

The aplication is layered as follows:  
  
![Alt text](/docs/images/SWLayers.jpg?raw=true "Flow")  
  
The project is separated in two different State Machines, one that handles all the network layer (Wifi connection) and the other one that handles all the protocol layer (TCP/IP connection). The State Machines are agnostic between each other and are also agnostic of the platform where they run.
Both state machines write and read to ring buffers. Those ring buffers are then connected to the UART Driver. 

The Wifi connection is triggered by the function ESP_WifiConnect. Once that function is call, the WIFI_SSID and WIFI_PASSWORD are read from the esp8266_cfg.h file. From there, the connection will be handled using AT commands by the Network layer State Machine.

Once the wifi connection is settled, the TCP/IP connection can be made. In order to do that the ESP_TCPSend function should be called with the host, port and payload to be send. From there, the connection and the write to the tcp socket will be handled by the TCP state machine.

Since we want to write to IFTTT, there is an extra layer of abstraction the IFTTT lib which creates the right payload to actually then call ESP_TCPSend. The IFTTT will be in charge of creating the POST HTTTP Request that will be the payload in the TCP layer. 

Since both state machines are agnostic of the platform they should be called periodically. The rate at which they are called can be condifured changing the BASE_TIME in the esp8266_cfg.h. Timming is not critical. Don't call the State Machine while running inside the ISR!!!

## How to configure the current project.

### Wifi Setings

In the file `esp8266_cfg.h` this configurations should be made:
- WIFI_SSSID
- WIFI_PASSWORD

### IFTTT Settings
In the file `ifttt_cfg.h` this configurations should be made:  
-IFTTT_SECRET_KEY  
To obtain that secret key you should visit your webhook ifttt settings page. 

Also, in this file is configured (using X-MACRO) the event names for the different webhooks programmed in the IFTTT platform.
The first column of the x-macro table is the ID (for the enum) for the event, the second column is the name used in the IFTTT webhook.

### Other important Settings.
