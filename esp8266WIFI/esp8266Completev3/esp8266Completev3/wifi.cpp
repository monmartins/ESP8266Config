#include "wifi.h"
// #include "functions.h"
#include "config.h"
#include "web.h"
// #include "Scan.h"


namespace wifi {
    // ===== PRIVATE ===== //
    wifi_mode_t   mode;
    ap_settings_t ap_settings;

    ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
    
    IPAddress local_IP WEB_IP_ADDR;
    IPAddress gateway GATEWAY;
    IPAddress subnet SUBNET;
    
    void startNewAP(const char *ssid, const char *password);
    void startMultiClientAP(const char *ssid, const char *password);
    void runMultiClientAP();

    void listWifi();
    void update();

    void setSSID(String ssid) {
        if (ssid.length() > 32) {
            debuglnF("ERROR: SSID longer than 32 characters");
        } else {
            strncpy(ap_settings.ssid, ssid.c_str(), 32);
        }
    }

    void setPassword(String password) {
        if (password.length() > 64) {
            debuglnF("ERROR: Password longer than 64 characters");
        } else if (password.length() < 8) {
            debuglnF("ERROR: Password must be at least 8 characters long");
        } else {
            strncpy(ap_settings.password, password.c_str(), 64);
        }
    }

    void setChannel(uint8_t ch) {
        if ((ch < 1) || (ch > 14)) {
            debuglnF("ERROR: Channel must be withing the range of 1-14");
        } else {
            ap_settings.channel = ch;
        }
    }

    void setHidden(bool hidden) {
        ap_settings.hidden = hidden;
    }

    void setCaptivePortal(bool captivePortal) {
        ap_settings.captive_portal = captivePortal;
    }
    // ===== PRIVATE ===== //
    void startNewAP(const char *ssid, const char *password){
        uint8_t channel=10;
        bool    hidden=false;
        bool    captive_portal=false;
        setSSID(ssid);
        setPassword(password);
        setChannel(channel);
        setHidden(hidden);
        setCaptivePortal(captive_portal);

        //
        // web::dns.setErrorReplyCode(DNSReplyCode::NoError);
        web::startDNS(local_IP);
        //
        
        Serial.print("Version 1.2 -- ");
        Serial.print("Setting soft-AP configuration ... ");
        Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

        WiFi.softAP(ssid, password);             // Start the access point
        Serial.print("Access Point \"");
        Serial.print(ssid);
        Serial.println("\" started");

        Serial.print("IP address:\t");
        Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
    }

    void listWifi(){
        int numberOfNetworks = WiFi.scanNetworks();

        for(int i =0; i<numberOfNetworks; i++){
            Serial.print("Network name: ");
            Serial.println(WiFi.SSID(i));
            Serial.print("Signal strength: ");
            Serial.println(WiFi.RSSI(i));
            Serial.println("-----------------------");
        }
    }

    void stopAP() {
        if (mode == wifi_mode_t::ap) {
            wifi_promiscuous_enable(0);
            WiFi.persistent(false);
            WiFi.disconnect(true);
            wifi_set_opmode(STATION_MODE);
            // prntln(W_STOPPED_AP);
            mode = wifi_mode_t::st;
        }
    }

    void resumeAP() {
        if (mode != wifi_mode_t::ap) {
            mode = wifi_mode_t::ap;
            wifi_promiscuous_enable(0);
            WiFi.softAPConfig(local_IP, local_IP, subnet);
            WiFi.softAP(ap_settings.ssid, ap_settings.password, ap_settings.channel, ap_settings.hidden);
            // prntln(W_STARTED_AP);
        }
    }

    void update() {
        // if ((mode != wifi_mode_t::off) && !scan.isScanning()) {
            web::loopServer();
            web::loopDNS();
            // web::dns.processNextRequest();
        // }
    }


    void startMultiClientAP(const char *ssid, const char *password){
        wifiMulti.addAP(ssid, password);
    }
    void runMultiClientAP(){
        while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
            delay(250);
            Serial.print('.');
        }
    }
}