#pragma once

extern "C" {
    #include "user_interface.h" //This is used for promiscuos mode wifi - wifi_promiscuous_enable
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <ESP8266WebServer.h>
// #include <DNSServer.h>
// #include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h> 
#include "functions.h"

// extern Scan   scan;
// extern CLI    cli;
// extern Attack attack;

typedef enum wifi_mode_t {
    off = 0,
    ap  = 1,
    st  = 2
} wifi_mode_t;

typedef struct ap_settings_t {
    char    path[33];
    char    ssid[33];
    char    password[65];
    uint8_t channel;
    bool    hidden;
    bool    captive_portal;
} ap_settings_t;

// Wifi was responsible for manage the AP and Station ESP8266 and list of WIfis
#define STATION_LIST_SIZE 60
namespace wifi {


    void startNewAP(const char *ssid, const char *password);
    void startMultiClientAP(const char *ssid, const char *password);
    void runMultiClientAP();


    void begin();
    String getMode();
    void printStatus();
    void startAP();
    void stopAP();
    void resumeAP();

    void listWifi();
    void update();


    
}