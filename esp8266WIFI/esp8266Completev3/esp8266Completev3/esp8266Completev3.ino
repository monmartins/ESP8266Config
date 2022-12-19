extern "C" {
  #include "user_interface.h"
}

#include "oui.h" // data_macs in Scan.cpp
#include "functions.h" // macBroadcast macMulticast macValid in Scan.cpp

#include "wifi.h"
#include "Scan.h"
#include "Attack.h"
#include "config.h"
#include "file.h"

uint32_t autosaveTime = 0;
uint32_t currentTime  = 0;

bool booted = false;
Accesspoints accesspoints;
Scan scan;
Attack attack;

void setup() {
  // for random generator
  randomSeed(os_random());
  Serial.begin(DEBUG_BAUD);
  Serial.println();
  // put your setup code here, to run once:
  file_spiffs::setupFILE();
  // load everything else
  config::load();
  accesspoints.Device_load();
  accesspoints.SSIDS_load();
  // create scan.json
  scan.setup();

  // start access point/web interface
  if (config::getWebSettings().enabled) wifi::startAP();

  // STARTED
  prntln(SETUP_STARTED);

  // version
  prntln(DEAUTHER_VERSION);
}

void loop() {
    currentTime = millis();
    
    wifi::update();  // manage access point
    attack.update(); // run attacks
    scan.update();   // run scan
    accesspoints.SSIDS_update();  // run random mode, if enabled
    // auto-save
    if (config::getAutosaveSettings().enabled
        && (currentTime - autosaveTime > config::getAutosaveSettings().time)) {
        autosaveTime = currentTime;
        accesspoints.Device_save(false);
        accesspoints.SSIDS_save(false);
        config::save(false);
    }
}
