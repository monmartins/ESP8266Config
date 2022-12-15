
#include "Scan.h"

#include "settings.h"
#include "wifi.h"

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
    // if(buf[12] != 0x08 && buf[12] != 0x88) return;

    uint8_t* macTo   = &buf[16];
    uint8_t* macFrom = &buf[22];

    if (macBroadcast(macTo) || macBroadcast(macFrom) || !macValid(macTo) || !macValid(macFrom) || macMulticast(macTo) ||
        macMulticast(macFrom)) return;

    int accesspointNum = findAccesspoint(macFrom);

    if (accesspointNum >= 0) {
        stations.add(macTo, accesspoints.getID(accesspointNum));
    } else {
        accesspointNum = findAccesspoint(macTo);

        if (accesspointNum >= 0) {
            stations.add(macFrom, accesspoints.getID(accesspointNum));
        }
    }
}