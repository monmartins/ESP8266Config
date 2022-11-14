
#pragma once

#include "ESP8266WiFi.h"
//
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

void startAP(const char *ssid, const char *password);

void listWifi();