
#include "oui.h" // data_macs in Scan.cpp
// #include "functions.h" // macBroadcast macMulticast macValid in Scan.cpp

#include "wifi.h"
// #include "language.h"
// #include "settings.h"
// #include "Names.h"
// #include "SSIDs.h"
#include "Scan.h"
// #include "Attack.h"
// #include "CLI.h"
// #include "DisplayUI.h"
#include "config.h"
#include "file.h"

uint32_t currentTime  = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {
}
