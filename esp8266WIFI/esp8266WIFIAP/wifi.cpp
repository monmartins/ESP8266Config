#include "wifi.h"


namespace wifi {
    // ===== PRIVATE ===== //
    wifi_mode_t   mode;
    ap_settings_t ap_settings;
    //
    const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
    IPAddress         apIP(192,168,65,1);    // Private network for server
    DNSServer         dnsServer;              // Create the DNS object
    //
    ESP8266WiFiMulti wifiMulti;
    boolean connectioWasAlive = true;
    HTTPClient https;
    WiFiClientSecure client;
    WiFiClient clientHttp;
    
    //
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
        // if DNSServer is started with "*" for domain name, it will reply with
        // provided IP to all DNS request
        dnsServer.start(DNS_PORT, "*", apIP);
        // Begin Access Point
        WiFi.mode(WIFI_AP_STA);
        uint8_t channel=10;
        bool    hidden=false;
        bool    captive_portal=false;
        setSSID(ssid);
        setPassword(password);
        setChannel(channel);
        setHidden(hidden);
        setCaptivePortal(captive_portal);

        IPAddress local_IP(192,168,65,1);
        IPAddress gateway(192,168,65,1);
        IPAddress subnet(255,255,255,0);

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
    void startMultiAP(){
        Serial.println("startMultiAP-----------------------");
        // Serial.println(WiFi.begin("brisa-2762758", "0bhyxvkk"));
        wifiMulti.addAP("brisa-2762758", "0bhyxvkk");
        // Serial.println(wifiMulti.addAP("MOB TELECOM", "S92y1Bv89r"));
        // wifiMulti.addAP("MOB TELECOM", "S92y1Bv89r");
        while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
          delay(250);
          Serial.print('.');
        }
        Serial.print("Connected to ");
        Serial.println(WiFi.SSID());              // Tell us what network we're connected to
        Serial.print("IP address:\t");
        Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
        //
        // client.setInsecure();
        // http.begin(wifiMulti,"http://jsonplaceholder.typicode.com/users");
        // http.addHeader("Content-Type", "text/plain");
        String url = "http://jsonplaceholder.typicode.com/users";
        if (https.begin(clientHttp, url)) {
          https.addHeader("Content-Type", "text/plain");
          int httpCode = https.GET();
          Serial.println("============== Response code: " + String(httpCode));
          if (httpCode > 0) {
            Serial.println(https.getString());
          }
          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
        Serial.println("startMultiAP-----------------------");
    }
    void monitorWiFi(){
        // wifiMulti.run();
        // Serial.println("monitorWiFi-----------------------");
        if (wifiMulti.run() != WL_CONNECTED)
        {
            if (connectioWasAlive == true)
            {
            connectioWasAlive = false;
            Serial.print("Looking for WiFi ");
            }
            Serial.print(".");
            delay(500);
        }
        else if (connectioWasAlive == false)
        {
            connectioWasAlive = true;
            Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
        }
        // Serial.println("monitorWiFi-----------------------");
    }
}
