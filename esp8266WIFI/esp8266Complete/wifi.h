#pragma once
#include <Arduino.h>

#include "ESP8266WiFi.h"
//
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "language.h"
#include "debug.h"
// #include "settings.h"
// #include "CLI.h"
// #include "Attack.h"
#include "Scan.h"
extern Scan   scan;

//Create modes of Wifi
typedef enum wifi_mode_t {
    off = 0, //WIFI OFF
    ap  = 1, //WIFI MODE AP ( ACCESS POINT )
    st  = 2 //WIFI MODE ST (STATION)
} wifi_mode_t;
//Settings to AP
typedef struct ap_settings_t {
    char    ssid[33];
    char    password[65];
    uint8_t channel;
    bool    hidden;
    bool    captive_portal;
} ap_settings_t;

namespace wifi {
    
    void startNewAP(const char *ssid, const char *password);
    void startMultiClientAP(const char *ssid, const char *password);
    void runMultiClientAP();

    void listWifi();
    void update();
}
