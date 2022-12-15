#include "Scan.h"

// #include "settings.h"
#include "wifi.h"

extern Accesspoints accesspoints;
// extern Stations     stations;
// extern Names names;
// extern SSIDs ssids;

Scan::Scan() {
    list = new SimpleList<uint16_t>;
}

bool Scan::isScanning() {
    return scanMode != SCAN_MODE_OFF;
}

bool Scan::isSniffing() {
    return scanMode == SCAN_MODE_STATIONS || scanMode == SCAN_MODE_SNIFFER;
}

void Scan::sniffer(uint8_t* buf, uint16_t len) {
    if (!isSniffing()) return;

    packets++;

    if (len < 28) return;  // drop frames that are too short to have a valid MAC header

    if ((buf[12] == 0xc0) || (buf[12] == 0xa0)) {
        tmpDeauths++;
        return;
    }

    // drop beacon frames, probe requests/responses and deauth/disassociation frames
    if ((buf[12] == 0x80) || (buf[12] == 0x40) || (buf[12] == 0x50) /* || buf[12] == 0xc0 || buf[12] == 0xa0*/) return;

    // only allow data frames
    if(buf[12] != 0x08 && buf[12] != 0x88) return;

    uint8_t* macTo   = &buf[16];
    uint8_t* macFrom = &buf[22];

    // esp8266Completev3.ino #include "oui.h" // data_macs in Scan.cpp
    // esp8266Completev3.ino #include "functions.h" // macBroadcast macMulticast macValid in Scan.cpp
    if (macBroadcast(macTo) || macBroadcast(macFrom) || !macValid(macTo) || !macValid(macFrom) || macMulticast(macTo) ||
        macMulticast(macFrom)) return;

    int accesspointNum = findAccesspoint(macFrom);

    if (accesspointNum >= 0) {
        accesspoints.Stations_add(macTo, accesspoints.getID(accesspointNum));
    } else {
        accesspointNum = findAccesspoint(macTo);

        if (accesspointNum >= 0) {
            accesspoints.Stations_add(macFrom, accesspoints.getID(accesspointNum));
        }
    }
}
void Scan::start(uint8_t mode) {
    start(mode, sniffTime, scan_continue_mode, continueTime, channelHop, wifi_channel);
}

void Scan::start(uint8_t mode, uint32_t time, uint8_t nextmode, uint32_t continueTime, bool channelHop,
                 uint8_t channel) {
    if (mode != SCAN_MODE_OFF) stop();

    setWifiChannel(channel, true);
    Scan::continueStartTime  = currentTime;
    Scan::snifferPacketTime  = continueStartTime;
    Scan::snifferOutputTime  = continueStartTime;
    Scan::continueTime       = continueTime;
    Scan::sniffTime          = time;
    Scan::channelHop         = channelHop;
    Scan::scanMode           = mode;
    Scan::scan_continue_mode = nextmode;

    if ((sniffTime > 0) && (sniffTime < 1000)) sniffTime = 1000;

    // Serial.printf("mode: %u, time: %u, continue-mode: %u, continueTime: %u, channelHop: %u, channel: %u\r\n", mode,
    // time, scan_continue_mode, continueTime, channelHop, channel);

    /* AP Scan */
    if ((mode == SCAN_MODE_APS) || (mode == SCAN_MODE_ALL)) {
        // remove old results
        accesspoints.removeAll();
        accesspoints.Stations_removeAll();
        // start AP scan
        prntln(SC_START_AP);
        WiFi.scanNetworks(true, true);
    }

    /* Station Scan */
    else if (mode == SCAN_MODE_STATIONS) {
        // start station scan
        if (accesspoints.count() < 1) {
            start(SCAN_MODE_ALL);
            // Serial.println(str(SC_ERROR_NO_AP));
            return;
        }
        snifferStartTime = currentTime;
        prnt(SC_START_CLIENT);

        if (sniffTime > 0) prnt(String(sniffTime / 1000) + S);
        else prnt(SC_INFINITELY);

        if (!channelHop) {
            prnt(SC_ON_CHANNEL);
            prnt(wifi_channel);
        }
        prntln();

        // enable sniffer
        wifi::stopAP();
        wifi_promiscuous_enable(true);
    }

    else if (mode == SCAN_MODE_SNIFFER) {
        deauths          = tmpDeauths;
        tmpDeauths       = 0;
        snifferStartTime = currentTime;
        prnt(SS_START_SNIFFER);

        if (sniffTime > 0) prnt(String(sniffTime / 1000) + S);
        else prnt(SC_INFINITELY);
        prnt(SC_ON_CHANNEL);
        prntln(channelHop ? str(SC_ONE_TO) + (String)14 : (String)wifi_channel);

        // enable sniffer
        wifi::stopAP();
        wifi_promiscuous_enable(true);
    }

    /* Stop scan */
    else if (mode == SCAN_MODE_OFF) {
        wifi_promiscuous_enable(false);

        // if (settings::getWebSettings().enabled) wifi::resumeAP();
        // prntln(SC_STOPPED);
        // save(true);

        if (scan_continue_mode != SCAN_MODE_OFF) {
            prnt(SC_RESTART);
            prnt(int(continueTime / 1000));
            prntln(SC_CONTINUE);
        }
    }

    /* ERROR */
    else {
        prnt(SC_ERROR_MODE);
        prntln(mode);
        return;
    }
}
void Scan::stop() {
    scan_continue_mode = SCAN_MODE_OFF;
    start(SCAN_MODE_OFF);
}

int Scan::findAccesspoint(uint8_t* mac) {
    for (int i = 0; i < accesspoints.count(); i++) {
        if (memcmp(accesspoints.getMac(i), mac, 6) == 0) return i;
    }
    return -1;
}

void Scan::save(bool force, String filePath) {
    String tmp = FILE_PATH;

    FILE_PATH = filePath;
    // save(true);
    FILE_PATH = tmp;
}

// void Scan::save(bool force) {
//     if (!(accesspoints.changed || stations.changed) && !force) return;

//     // Accesspoints
//     String buf = String(OPEN_CURLY_BRACKET) + String(DOUBLEQUOTES) + str(SC_JSON_APS) + String(DOUBLEQUOTES) + String(
//         DOUBLEPOINT) + String(OPEN_BRACKET); // {"aps":[

//     if (!writeFile(FILE_PATH, buf)) {        // overwrite old file
//         prnt(F_ERROR_SAVING);
//         prntln(FILE_PATH);
//         return;
//     }

//     buf = String(); // clear buffer
//     uint32_t apCount = accesspoints.count();

//     for (uint32_t i = 0; i < apCount; i++) {
//         buf += String(OPEN_BRACKET) + String(DOUBLEQUOTES) + escape(accesspoints.getSSID(i)) + String(DOUBLEQUOTES) +
//                String(COMMA);                                                                                    // ["ssid",
//         buf += String(DOUBLEQUOTES) + escape(accesspoints.getNameStr(i)) + String(DOUBLEQUOTES) + String(COMMA); // "name",
//         buf += String(accesspoints.getCh(i)) + String(COMMA);                                                    // 1,
//         buf += String(accesspoints.getRSSI(i)) + String(COMMA);                                                  // -30,
//         buf += String(DOUBLEQUOTES) + accesspoints.getEncStr(i) + String(DOUBLEQUOTES) + String(COMMA);          // "wpa2",
//         buf += String(DOUBLEQUOTES) + accesspoints.getMacStr(i) + String(DOUBLEQUOTES) + String(COMMA);          // "00:11:22:00:11:22",
//         buf += String(DOUBLEQUOTES) + accesspoints.getVendorStr(i) + String(DOUBLEQUOTES) + String(COMMA);       // "vendor",
//         buf += b2s(accesspoints.getSelected(i)) + String(CLOSE_BRACKET);                                         // false]

//         if (i < apCount - 1) buf += String(COMMA);                                                               // ,

//         if (buf.length() >= 1024) {
//             if (!appendFile(FILE_PATH, buf)) {
//                 prnt(F_ERROR_SAVING);
//                 prntln(FILE_PATH);
//                 return;
//             }

//             buf = String(); // clear buffer
//         }
//     }

//     // Stations
//     buf += String(CLOSE_BRACKET) + String(COMMA) + String(DOUBLEQUOTES) + str(SC_JSON_STATIONS) + String(DOUBLEQUOTES) +
//            String(DOUBLEPOINT) + String(OPEN_BRACKET); // ],"stations":[;
//     uint32_t stationCount = stations.count();

//     for (uint32_t i = 0; i < stationCount; i++) {
//         buf += String(OPEN_BRACKET) + String(DOUBLEQUOTES) + stations.getMacStr(i) + String(DOUBLEQUOTES) +
//                String(COMMA);                                                                          // ["00:11:22:00:11:22",
//         buf += String(stations.getCh(i)) + String(COMMA);                                              // 1,
//         buf += String(DOUBLEQUOTES) + stations.getNameStr(i) + String(DOUBLEQUOTES) + String(COMMA);   // "name",
//         buf += String(DOUBLEQUOTES) + stations.getVendorStr(i) + String(DOUBLEQUOTES) + String(COMMA); // "vendor",
//         buf += String(*stations.getPkts(i)) + String(COMMA);                                           // 123,
//         buf += String(stations.getAP(i)) + String(COMMA);                                              // 0,
//         buf += String(DOUBLEQUOTES) + stations.getTimeStr(i) + String(DOUBLEQUOTES) + String(COMMA);   // "<1min",
//         buf += b2s(stations.getSelected(i)) + String(CLOSE_BRACKET);                                   // false]

//         if (i < stationCount - 1) buf += String(COMMA);                                                // ,

//         if (buf.length() >= 1024) {
//             if (!appendFile(FILE_PATH, buf)) {
//                 prnt(F_ERROR_SAVING);
//                 prntln(FILE_PATH);
//                 return;
//             }

//             buf = String(); // clear buffer
//         }
//     }

//     buf += String(CLOSE_BRACKET) + String(CLOSE_CURLY_BRACKET); // ]}

//     if (!appendFile(FILE_PATH, buf)) {
//         prnt(F_ERROR_SAVING);
//         prntln(FILE_PATH);
//         return;
//     }

//     accesspoints.changed = false;
//     stations.changed     = false;
//     prnt(SC_SAVED_IN);
//     prntln(FILE_PATH);
// }