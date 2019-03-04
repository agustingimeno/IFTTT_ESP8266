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

Explanation

## How to configure the current project.

### Wifi Setings
### IFTTT Settings
### Other important Settings.
