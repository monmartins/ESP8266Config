#pragma once

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

namespace web {
    
    void handleWifi();
    void handleWifiList();
    void handleRoot();

    void handleNotFound();
    String getContentType(String filename);

    bool handleFileRead(String path);

    void startWeb();

    void loopServer();

    void startDNS(IPAddress local_IP);
    void loopDNS();
    
}