
#pragma once

#include "ESP8266WiFi.h"
//
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <ArduinoJson.h>

void handleWifi();
void handleWifiList();
void handleRoot();

void handleNotFound();
String getContentType(String filename);

bool handleFileRead(String path);

void startWebServer();

void loopServer();