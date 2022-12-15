#include "wifi.h"    // Include the Wi-Fi library

const char *ssid = "HOME_S"; // The name of the Wi-Fi network that will be created
const char *password = "thereiskaust5049";   // The password required to connect to it, leave blank for an open network

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');          
  wifi::startNewAP(ssid,password);// Start the access point
  wifi::startMultiAP();// Start multi access point
}

void loop() {}// wifi::monitorWiFi();}
