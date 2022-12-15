
#include "language.h"
#include "oui.h" //Used by functions.h
#include "functions.h" //Functions to use entire program
#include "settings.h" //Save all settings, wifi attacks, webserver, IPFS
#include "Scan.h"
#include "Attack.h" //Save all Wifi attacks 
#include "webServer.h" //Controller of webServer
#include "wifi.h" //Manage Wifi esp8266
#include <FS.h>   // Include the SPIFFS library
// #include <ArduinoJson.h>
//#include <ConfigStorage.h> // https://github.com/Tost69/ConfigStorage - https://github.com/Tost69/ConfigStorage/blob/main/examples/ConfigStorage_ESP32/ConfigStorage_ESP32.ino
 


//FS
// String getContentType(String filename); // convert the file extension to the MIME type
// bool handleFileRead(String path);       // send the right file to the client (if it exists)
Scan   scan;

const char *ssid = "ESP8266-Deauth"; // The name of the Wi-Fi network that will be created
uint32_t currentTime  = 0; //Used by Attack.h
void setup() {
    // for random generator
    randomSeed(os_random());

    Serial.begin(115200);
    delay(10);
    Serial.println('\n');
    const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open  

    wifi::startNewAP(ssid,password);
    SPIFFS.begin();                           // Start the SPI Flash Files System
    startWebServer();
    // get time
    currentTime = millis();
    wifi_set_promiscuous_rx_cb([](uint8_t* buf, uint16_t len) {
        // scan.sniffer(buf, len);
    });
 
}

 
// void loop() {}

void loop(void){
  loopServer();
  MDNS.update();
  wifi::update();  // manage access point
}
