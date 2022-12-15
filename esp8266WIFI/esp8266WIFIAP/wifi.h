#include <ESP8266WiFi.h> 
#include <ESP8266WiFiMulti.h> 
#include "debug.h"
#include "DNSServer.h"                  // Patched lib
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>

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

    void listWifi();
    void startMultiAP();
    void monitorWiFi();
}
