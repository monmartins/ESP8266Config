

#include "Accesspoints.h"
#include "file.h"

Accesspoints::Accesspoints() {
    list = new SimpleList<AP>;
    list_Station = new SimpleList<Station>;
    list_Device = new SimpleList<Device>;
    list_SSID =  new SimpleList<SSID>;
}

void Accesspoints::sort() {
    list->setCompare([](AP& a, AP& b) -> int {
        if (WiFi.RSSI(a.id) > WiFi.RSSI(b.id)) return -1;

        if (WiFi.RSSI(a.id) == WiFi.RSSI(b.id)) return 0;

        return 1;
    });
    list->sort();
    changed = true;
}

void Accesspoints::sortAfterChannel() {
    list->setCompare([](AP& a, AP& b) -> int {
        if (WiFi.channel(a.id) < WiFi.channel(b.id)) return -1;

        if (WiFi.channel(a.id) == WiFi.channel(b.id)) return 0;

        return 1;
    });
    list->sort();
    changed = true;
}

void Accesspoints::add(uint8_t id, bool selected) {
    list->add(AP{ id, selected });
    changed = true;
}

void Accesspoints::printAll() {
    prntln(AP_HEADER);
    int c = count();

    if (c == 0) prntln(AP_LIST_EMPTY);
    else
        for (int i = 0; i < c; i++) print(i, i == 0, i == c - 1);
}

void Accesspoints::printSelected() {
    prntln(AP_HEADER);
    int max = selected();

    if (selected() == 0) {
        prntln(AP_NO_AP_SELECTED);
        return;
    }
    int c = count();
    int j = 0;

    for (int i = 0; i < c && j < max; i++) {
        if (getSelected(i)) {
            print(i, j == 0, j == max - 1);
            j++;
        }
    }
}

void Accesspoints::print(int num) {
    print(num, true, true);
}

void Accesspoints::print(int num, bool header, bool footer) {
    if (!check(num)) return;

    if (header) {
        prntln(AP_TABLE_HEADER);
        prntln(AP_TABLE_DIVIDER);
    }
    prnt(leftRight(String(), (String)num, 2));
    prnt(leftRight(String(SPACE) + getSSID(num), String(), 33));
    prnt(leftRight(String(SPACE) + getNameStr(num), String(), 17));
    prnt(leftRight(String(SPACE), (String)getCh(num), 3));
    prnt(leftRight(String(SPACE), (String)getRSSI(num), 5));
    prnt(leftRight(String(SPACE), getEncStr(num), 5));
    prnt(leftRight(String(SPACE) + getMacStr(num), String(), 18));
    prnt(leftRight(String(SPACE) + getVendorStr(num), String(), 9));
    prntln(leftRight(String(SPACE) + getSelectedStr(num), String(), 9));

    if (footer) {
        prntln(AP_TABLE_DIVIDER);
    }
}

String Accesspoints::getSSID(int num) {
    if (!check(num)) return String();

    if (getHidden(num)) {
        return str(AP_HIDDE_SSID);
    } else {
        String ssid = WiFi.SSID(getID(num));
        ssid = ssid.substring(0, 32);
        ssid = fixUtf8(ssid);
        return ssid;
    }
}

String Accesspoints::getNameStr(int num) {
    if (!check(num)) return String();

    // return names.find(getMac(num));
}

uint8_t Accesspoints::getCh(int num) {
    if (!check(num)) return 0;

    return WiFi.channel(getID(num));
}

int Accesspoints::getRSSI(int num) {
    if (!check(num)) return 0;

    return WiFi.RSSI(getID(num));
}

uint8_t Accesspoints::getEnc(int num) {
    if (!check(num)) return 0;

    return WiFi.encryptionType(getID(num));
}

String Accesspoints::getEncStr(int num) {
    if (!check(num)) return String();

    switch (getEnc(num)) {
        case ENC_TYPE_NONE:
            return String(DASH);

            break;

        case ENC_TYPE_WEP:
            return str(AP_WEP);

            break;

        case ENC_TYPE_TKIP:
            return str(AP_WPA);

            break;

        case ENC_TYPE_CCMP:
            return str(AP_WPA2);

            break;

        case ENC_TYPE_AUTO:
            return str(AP_AUTO);

            break;
    }
    return String(QUESTIONMARK);
}

String Accesspoints::getSelectedStr(int num) {
    return b2a(getSelected(num));
}

uint8_t* Accesspoints::getMac(int num) {
    if (!check(num)) return 0;

    return WiFi.BSSID(getID(num));
}

String Accesspoints::getMacStr(int num) {
    if (!check(num)) return String();

    uint8_t* mac = getMac(num);

    return bytesToStr(mac, 6);
}

String Accesspoints::getVendorStr(int num) {
    if (!check(num)) return String();

    return searchVendor(getMac(num));
}

bool Accesspoints::getHidden(int num) {
    if (!check(num)) return false;

    return WiFi.isHidden(getID(num));
}

bool Accesspoints::getSelected(int num) {
    if (!check(num)) return false;

    return list->get(num).selected;
}

uint8_t Accesspoints::getID(int num) {
    if (!check(num)) return -1;

    return list->get(num).id;
}

void Accesspoints::select(int num) {
    if (!check(num)) return;

    internal_select(num);

    prnt(AP_SELECTED);
    prntln(getSSID(num));

    changed = true;
}

void Accesspoints::deselect(int num) {
    if (!check(num)) return;

    internal_deselect(num);

    prnt(AP_DESELECTED);
    prntln(getSSID(num));

    changed = true;
}

void Accesspoints::remove(int num) {
    if (!check(num)) return;

    prnt(AP_REMOVED);
    prntln(getSSID(num));

    internal_remove(num);

    changed = true;
}

void Accesspoints::select(String ssid) {
    for (int i = 0; i < list->size(); i++) {
        if (getSSID(i).equalsIgnoreCase(ssid)) select(i);
    }
}

void Accesspoints::deselect(String ssid) {
    for (int i = 0; i < list->size(); i++) {
        if (getSSID(i).equalsIgnoreCase(ssid)) deselect(i);
    }
}

void Accesspoints::remove(String ssid) {
    for (int i = 0; i < list->size(); i++) {
        if (getSSID(i).equalsIgnoreCase(ssid)) remove(i);
    }
}

void Accesspoints::selectAll() {
    for (int i = 0; i < count(); i++) list->replace(i, AP{ list->get(i).id, true });
    prntln(AP_SELECTED_ALL);
    changed = true;
}

void Accesspoints::deselectAll() {
    for (int i = 0; i < count(); i++) list->replace(i, AP{ list->get(i).id, false });
    prntln(AP_DESELECTED_ALL);
    changed = true;
}

void Accesspoints::removeAll() {
    while (count() > 0) internal_remove(0);
    prntln(AP_REMOVED_ALL);
    changed = true;
}

int Accesspoints::find(uint8_t id) {
    int s = list->size();

    for (int i = 0; i < s; i++) {
        if (list->get(i).id == id) return i;
    }
    return -1;
}

int Accesspoints::count() {
    return list->size();
}

int Accesspoints::selected() {
    int c = 0;

    for (int i = 0; i < list->size(); i++) c += list->get(i).selected;
    return c;
}

bool Accesspoints::check(int num) {
    if (internal_check(num)) return true;

    prnt(AP_NO_AP_ERROR);
    prntln((String)num);
    return false;
}

bool Accesspoints::internal_check(int num) {
    return num >= 0 && num < count();
}

void Accesspoints::internal_select(int num) {
    list->replace(num, AP{ list->get(num).id, true });
}

void Accesspoints::internal_deselect(int num) {
    list->replace(num, AP{ list->get(num).id, false });
}

void Accesspoints::internal_remove(int num) {
    list->remove(num);
}

// Station 

void Accesspoints::Stations_sort() {
    list_Station->setCompare([](Station& a, Station& b) -> int {
        if (*(a.pkts) > *(b.pkts)) return -1;

        if (*(a.pkts) == *(b.pkts)) return 0;

        return 1;
    });
    list_Station->sort();
}

void Accesspoints::Stations_sortAfterChannel() {
    list_Station->setCompare([](Station& a, Station& b) -> int {
        if (a.ch < b.ch) return -1;

        if (a.ch == b.ch) return 0;

        return 1;
    });
    list_Station->sort();
}
uint8_t* Accesspoints::Stations_getMac(int num){
    if (!check(num)) return 0;

    return list_Station->get(num).mac;
}

int Accesspoints::Stations_findStation(uint8_t* mac) {
    int c = count();

    for (int i = 0; i < c; i++) {
        if (memcmp(Stations_getMac(i), mac, 6) == 0) return i;
    }
    return -1;
}

int Accesspoints::Stations_getAP(int num) {
    if (!Stations_check(num)) return -1;

    return find(list_Station->get(num).ap);
}

String Accesspoints::Stations_getAPStr(int num) {
    if (Stations_getAP(num) < 0) return String();

    return getSSID(Stations_getAP(num));
}

void Accesspoints::Stations_select(String ssid) {
    for (int i = 0; i < list_Station->size(); i++) {
        if (Stations_getAPStr(i).equalsIgnoreCase(ssid)) Stations_select(i);
    }
}

void Accesspoints::Stations_deselect(String ssid) {
    for (int i = 0; i < list_Station->size(); i++) {
        if (Stations_getAPStr(i).equalsIgnoreCase(ssid)) Stations_deselect(i);
    }
}

void Accesspoints::Stations_remove(String ssid) {
    for (int i = 0; i < list_Station->size(); i++) {
        if (Stations_getAPStr(i).equalsIgnoreCase(ssid)) Stations_remove(i);
    }
}

void Accesspoints::Stations_selectAll() {
    for (int i = 0; i < Stations_count(); i++) Stations_internal_select(i);
    prntln(ST_SELECTED_ALL);
    changed = true;
}

void Accesspoints::Stations_deselectAll() {
    for (int i = 0; i < Stations_count(); i++) Stations_internal_deselect(i);
    prntln(ST_DESELECTED_ALL);
    changed = true;
}
void Accesspoints::Stations_removeAll() {
    Stations_internal_removeAll();
    prntln(ST_CLEARED_LIST);
    Stations_changed = true;
}

void Accesspoints::Stations_removeOldest() {
    int oldest = 0;
    int c      = Stations_count();

    for (int i = 1; i < c; i++) {
        if (*Stations_getTime(i) > *Stations_getTime(oldest)) oldest = i;
    }
    Stations_internal_remove(oldest);
    changed = true;
}

String Accesspoints::Stations_getNameStr(int num) {
    if (!Stations_check(num)) return String();

    return Device_find(Stations_getMac(num));
}

bool Accesspoints::Stations_hasName(int num) {
    if (!Stations_check(num)) return false;

    return Device_findID(getMac(num)) >= 0;
}



String Accesspoints::Stations_getMacStr(int num) {
    if (!Stations_check(num)) return String();

    uint8_t* mac = Stations_getMac(num);

    return bytesToStr(mac, 6);
}

String Accesspoints::Stations_getMacVendorStr(int num) {
    String value;

    if (Stations_check(num)) {
        value = Stations_getVendorStr(num) + ":";
        uint8_t* mac = Stations_getMac(num);

        for (int i = 3; i < 6; i++) {
            if (mac[i] < 0x10) value += "0";
            value += String(mac[i], HEX);

            if (i < 5) value += ":";
        }
    }
    return value;
}

uint32_t* Accesspoints::Stations_getPkts(int num) {
    if (!Stations_internal_check(num)) return NULL;

    return list_Station->get(num).pkts;
}

uint32_t* Accesspoints::Stations_getTime(int num) {
    if (!Stations_internal_check(num)) return NULL;

    return list_Station->get(num).time;
}

String Accesspoints::Stations_getTimeStr(int num) {
    if (!Stations_check(num)) return String();

    uint32_t difference = currentTime - *Stations_getTime(num);

    if (difference < 1000) return str(ST_SMALLER_ONESEC);
    else if (difference < 60000) return str(ST_SMALLER_ONEMIN);
    else {
        uint32_t minutes = difference / 60000;

        if (minutes > 60) return str(ST_BIGER_ONEHOUR);
        else return (String)minutes + str(STR_MIN);
    }
}

String Accesspoints::Stations_getVendorStr(int num) {
    if (!Stations_check(num)) return String();

    return searchVendor(list_Station->get(num).mac);
}
uint8_t* Accesspoints::Stations_getAPMac(int num) {
    if (!Stations_check(num)) return 0;

    return WiFi.BSSID(list_Station->get(num).ap);
}

String Accesspoints::Stations_getAPMacStr(int num) {
    if (!Stations_check(num)) return String();

    uint8_t* mac = Stations_getAPMac(num);

    return bytesToStr(mac, 6);
}


uint8_t Accesspoints::Stations_getCh(int num) {
    if (!Stations_check(num)) return 0;

    return list_Station->get(num).ch;
}

bool Accesspoints::Stations_check(int num) {
    if (Stations_internal_check(num)) {
        return true;
    } else {
        prnt(ST_ERROR_ID);
        prntln(num);
        return false;
    }
}

bool Accesspoints::Stations_getSelected(int num) {
    if (!Stations_check(num)) return false;

    return list_Station->get(num).selected;
}

int Accesspoints::Stations_count() {
    return list_Station->size();
}

int Accesspoints::Stations_selected() {
    int num = 0;

    for (int i = 0; i < count(); i++)
        if (Stations_getSelected(i)) num++;
    return num;
}

void Accesspoints::Stations_printAll() {
    prntln(ST_HEADER);
    int c = count();

    if (c == 0) prntln(ST_LIST_EMPTY);
    else
        for (int i = 0; i < c; i++) print(i, i == 0, i == c - 1);
}

void Accesspoints::Stations_printSelected() {
    prntln(ST_HEADER);
    int max = Stations_selected();
    int c   = count();

    if (max == 0) {
        prntln(ST_NO_DEVICES_SELECTED);
        return;
    }

    for (int i = 0, j = 0; i < c && j < max; i++) {
        if (Stations_getSelected(i)) {
            print(i, j == 0, j == max - 1);
            j++;
        }
    }
}

void Accesspoints::Stations_print(int num) {
    print(num, true, true);
}

void Accesspoints::Stations_print(int num, bool header, bool footer) {
    if (!Stations_check(num)) return;

    if (header) {
        prntln(ST_TABLE_HEADER);
        prntln(ST_TABLE_DIVIDER);
    }

    prnt(leftRight(String(), (String)num, 2));
    prnt(leftRight(String(SPACE) + Stations_getMacStr(num), String(), 18));
    prnt(leftRight(String(SPACE), (String)Stations_getCh(num), 3));
    prnt(leftRight(String(SPACE) + Stations_getNameStr(num), String(), 17));
    prnt(leftRight(String(SPACE) + Stations_getVendorStr(num), String(), 9));
    prnt(leftRight(String(SPACE), (String) * Stations_getPkts(num), 9));
    prnt(leftRight(String(SPACE) + Stations_getAPStr(num), String(), 33));
    prnt(leftRight(String(SPACE) + Stations_getTimeStr(num), String(), 10));
    prntln(leftRight(String(SPACE) + Stations_getSelectedStr(num), String(), 9));

    if (footer) prntln(ST_TABLE_DIVIDER);
}



void Accesspoints::Stations_add(uint8_t* mac, int accesspointNum) {
    int stationNum = Stations_findStation(mac);

    if (stationNum < 0) {
        Stations_internal_add(mac, accesspointNum);
        // print(list->size() - 1, list->size() == 1, false);
    } else {
        *Stations_getPkts(stationNum) += 1;
        *Stations_getTime(stationNum)  = currentTime;
    }
    Stations_changed = true;
}


bool Accesspoints::Stations_internal_check(int num) {
    return num >= 0 && num < Stations_count();
}

void Accesspoints::Stations_internal_select(int num) {
    Station changedStation = list_Station->get(num);

    changedStation.selected = true;
    list_Station->replace(num, changedStation);
}

void Accesspoints::Stations_internal_deselect(int num) {
    Station changedStation = list_Station->get(num);

    changedStation.selected = false;
    list_Station->replace(num, changedStation);
}

void Accesspoints::Stations_internal_remove(int num) {
    free(Stations_getMac(num));
    free(Stations_getPkts(num));
    free(Stations_getTime(num));
    list_Station->remove(num);
}

void Accesspoints::Stations_internal_add(uint8_t* mac, int accesspointNum) {
    if (Stations_count() >= STATION_LIST_SIZE) Stations_removeOldest();

    Station newStation;

    newStation.ap       = accesspointNum;
    newStation.ch       = wifi_channel;
    newStation.mac      = (uint8_t*)malloc(6);
    newStation.pkts     = (uint32_t*)malloc(sizeof(uint32_t));
    newStation.time     = (uint32_t*)malloc(sizeof(uint32_t));
    newStation.selected = false;

    memcpy(newStation.mac, mac, 6);
    *newStation.pkts = 1;
    *newStation.time = currentTime;

    list_Station->add(newStation);
}

void Accesspoints::Stations_internal_removeAll() {
    int c = count();

    for (int i = 0; i < c; i++) {
        free(Stations_getMac(i));
        free(Stations_getPkts(i));
        free(Stations_getTime(i));
    }
    list_Station->clear();
}

// Device 


int Accesspoints::Device_findID(uint8_t* mac) {
    for (int i = 0; i < list_Device->size(); i++) {
        if (memcmp(mac, list_Device->get(i).mac, 6) == 0) return i;
    }

    return -1;
}

String Accesspoints::Device_find(uint8_t* mac) {
    int num = Device_findID(mac);

    if (num >= 0) return Device_getName(num);
    else return String();
}

String Accesspoints::Device_getName(int num) {
    if (!Device_check(num)) return String();

    return String(list_Device->get(num).name);
}
bool Accesspoints::Device_check(int num) {
    if (Device_internal_check(num)) return true;

    prnt(N_ERROR_NOT_FOUND);
    prntln(num);
    return false;
}
void Accesspoints::Device_load() {
    Device_internal_removeAll();

    DynamicJsonDocument jsonBuffer(4000);
    // DynamicJsonBuffer jsonBuffer(4000);

    file_spiffs::checkFile(DEVICE_FILE_PATH, String(OPEN_BRACKET) + String(CLOSE_BRACKET));
    
    JsonArray arr = parseJSONFile(DEVICE_FILE_PATH, jsonBuffer);

    // for (uint32_t i = 0; i < arr.size() && i < DEVICE_LIST_SIZE; i++) {
    //     JsonArray tmpArray = arr.get<JsonVariant>(i);
    //     Device_internal_add(tmpArray.get<String>(0), tmpArray.get<String>(2), tmpArray.get<String>(3), tmpArray.get<uint8_t>(4), false);
    //     Device_sort();
    // }
    for(JsonArray v : arr) {
        Device_internal_add(v[0].as<String>(), v[2].as<String>(), v[3].as<String>(), v[4].as<uint8_t>(), false);
        Device_sort();
    }

    prnt(N_LOADED);
    prntln(DEVICE_FILE_PATH);
}

void Accesspoints::Device_load(String filepath) {
    String tmp = DEVICE_FILE_PATH;

    DEVICE_FILE_PATH = filepath;
    Device_load();
    DEVICE_FILE_PATH = tmp;
}

void Accesspoints::Device_save(bool force) {
    if (!Device_force && !Device_changed) {
        return;

        prntln(N_SAVED);
    }

    String buf = String(OPEN_BRACKET); // [

    if (!file_spiffs::writeFile(DEVICE_FILE_PATH, buf)) {
        prnt(F_ERROR_SAVING);
        prntln(DEVICE_FILE_PATH);
        return;
    }

    buf = String();

    String name;
    int    c = Device_count();

    for (int i = 0; i < c; i++) {
        name = escape(Device_getName(i));

        buf += String(OPEN_BRACKET) + String(DOUBLEQUOTES) + Device_getMacStr(i) + String(DOUBLEQUOTES) + String(COMMA); // ["00:11:22:00:11:22",
        buf += String(DOUBLEQUOTES) + Device_getVendorStr(i) + String(DOUBLEQUOTES) + String(COMMA);                     // "vendor",
        buf += String(DOUBLEQUOTES) + name + String(DOUBLEQUOTES) + String(COMMA);                                // "name",
        buf += String(DOUBLEQUOTES) + Device_getBssidStr(i) + String(DOUBLEQUOTES) + String(COMMA);                      // "00:11:22:00:11:22",
        buf += String(Device_getCh(i)) + String(COMMA);                                                                  // 1,
        buf += b2s(Device_getSelected(i)) + String(CLOSE_BRACKET);                                                       // false]

        if (i < c - 1) buf += COMMA;                                                                              // ,

        if (buf.length() >= 1024) {
            if (!file_spiffs::appendFile(DEVICE_FILE_PATH, buf)) {
                prnt(F_ERROR_SAVING);
                prntln(DEVICE_FILE_PATH);
                return;
            }

            buf = String();
        }
    }

    buf += String(CLOSE_BRACKET); // ]

    if (!file_spiffs::appendFile(DEVICE_FILE_PATH, buf)) {
        prnt(F_ERROR_SAVING);
        prntln(DEVICE_FILE_PATH);
        return;
    }

    prnt(N_SAVED);
    prntln(DEVICE_FILE_PATH);
    changed = false;
}

void Accesspoints::Device_save(bool force, String filepath) {
    String tmp = DEVICE_FILE_PATH;

    DEVICE_FILE_PATH = filepath;
    Device_save(force);
    DEVICE_FILE_PATH = tmp;
}

void Accesspoints::Device_sort() {
    list_Device->setCompare([](Device& a, Device& b) -> int {
        return memcmp(a.mac, b.mac, 6);
    });
    list_Device->sort();
}

void Accesspoints::Device_removeAll() {
    Device_internal_removeAll();
    prntln(N_REMOVED_ALL);
    Device_changed = true;
}
//
uint8_t* Accesspoints::Device_getMac(int num) {
    if (!Device_check(num)) return NULL;

    return list_Device->get(num).mac;
}

uint8_t* Accesspoints::Device_getBssid(int num) {
    if (!Device_check(num)) return NULL;

    return list_Device->get(num).apBssid;
}

String Accesspoints::Device_getMacStr(int num) {
    if (!Device_check(num)) return String();

    uint8_t* mac = getMac(num);

    return bytesToStr(mac, 6);
}

String Accesspoints::Device_getBssidStr(int num) {
    String value;

    if (Device_getBssid(num) != NULL) {
        uint8_t* mac = Device_getBssid(num);

        for (int i = 0; i < 6; i++) {
            if (mac[i] < 0x10) value += ZERO;
            value += String(mac[i], HEX);

            if (i < 5) value += DOUBLEPOINT;
        }
    }
    return value;
}

String Accesspoints::Device_getVendorStr(int num) {
    if (!Device_check(num)) return String();

    return searchVendor(list_Device->get(num).mac);
}

String Accesspoints::Device_getSelectedStr(int num) {
    return b2a(getSelected(num));
}

uint8_t Accesspoints::Device_getCh(int num) {
    if (!Device_check(num)) return 1;

    return list_Device->get(num).ch;
}

bool Accesspoints::Device_getSelected(int num) {
    if (!Device_check(num)) return false;

    return list_Device->get(num).selected;
}

//
bool Accesspoints::Device_isStation(int num) {
    return Device_getBssid(num) != NULL;
}
//
void Accesspoints::Device_setName(int num, String name) {
    if (!Device_check(num)) return;

    Device_internal_add(Device_getMac(num), name, Device_getBssid(num), Device_getCh(num), Device_getSelected(num));

    prntln(N_CHANGED_NAME);

    Device_internal_remove(num);
    Device_sort();
    Device_changed = true;
}

void Accesspoints::Device_setMac(int num, String macStr) {
    if (!Device_check(num)) return;

    uint8_t mac[6];

    strToMac(macStr, mac);
    Device_internal_add(mac, Device_getName(num), Device_getBssid(num), Device_getCh(num), Device_getSelected(num));
    prntln(N_CHANGED_MAC);
    Device_internal_remove(num);
    Device_sort();
    Device_changed = true;
}

void Accesspoints::Device_setCh(int num, uint8_t ch) {
    if (!Device_check(num)) return;

    Device_internal_add(Device_getMac(num), Device_getName(num), Device_getBssid(num), ch, Device_getSelected(num));
    prntln(N_CHANGED_CH);
    Device_internal_remove(num);
    Device_sort();
    Device_changed = true;
}

void Accesspoints::Device_setBSSID(int num, String bssidStr) {
    if (!Device_check(num)) return;

    uint8_t mac[6];

    strToMac(bssidStr, mac);
    Device_internal_add(Device_getMac(num), Device_getName(num), mac, Device_getCh(num), Device_getSelected(num));
    prntln(N_CHANGED_BSSID);
    Device_internal_remove(num);
    Device_sort();
    Device_changed = true;
}

int Accesspoints::Device_count() {
    return list_Device->size();
}
int Accesspoints::Device_selected() {
    int num = 0;

    for (int i = 0; i < count(); i++)
        if (getSelected(i)) num++;
    return num;
}
//
int Accesspoints::Device_stations() {
    int num = 0;

    for (int i = 0; i < Device_count(); i++)
        if (Device_isStation(i)) num++;
    return num;
}

bool Accesspoints::Device_internal_check(int num) {
    return num >= 0 && num < Device_count();
}


void Accesspoints::Device_internal_add(uint8_t* mac, String name, uint8_t* bssid, uint8_t ch, bool selected) {
    uint8_t* deviceMac = (uint8_t*)malloc(6);

    if (name.length() > DEVICE_MAX_LENGTH) name = name.substring(0, DEVICE_MAX_LENGTH);
    char* deviceName     = (char*)malloc(name.length() + 1);
    uint8_t* deviceBssid = NULL;

    name = fixUtf8(name);

    memcpy(deviceMac, mac, 6);
    strcpy(deviceName, name.c_str());

    if (bssid) {
        deviceBssid = (uint8_t*)malloc(6);
        memcpy(deviceBssid, bssid, 6);
    }

    if ((ch < 1) || (ch > 14)) ch = 1;

    Device newDevice;

    newDevice.mac      = deviceMac;
    newDevice.name     = deviceName;
    newDevice.apBssid  = deviceBssid;
    newDevice.ch       = ch;
    newDevice.selected = selected;

    list_Device->add(newDevice);
}

void Accesspoints::Device_internal_add(String macStr, String name, String bssidStr, uint8_t ch, bool selected) {
    uint8_t mac[6];

    if (!strToMac(macStr, mac)) return;

    if (bssidStr.length() == 17) {
        uint8_t bssid[6];
        strToMac(bssidStr, bssid);
        Device_internal_add(mac, name, bssid, ch, selected);
    } else {
        Device_internal_add(mac, name, NULL, ch, selected);
    }
}
void Accesspoints::Device_internal_removeAll() {
    while (Device_count() > 0) {
        free(list_Device->get(0).mac);
        free(list_Device->get(0).name);

        if (list_Device->get(0).apBssid) free(list_Device->get(0).apBssid);
        list_Device->remove(0);
    }
}


//SSID

void Accesspoints::SSIDS_load() {
    SSIDS_internal_removeAll();

    DynamicJsonDocument jsonBuffer(4000);
    // DynamicJsonBuffer jsonBuffer(4000);

    file_spiffs::checkFile(SSIDS_FILE_PATH, str(SS_JSON_DEFAULT));
    JsonObject obj = parseJSONFile(SSIDS_FILE_PATH, jsonBuffer);
    // JsonArray arr = obj.get<JsonArray>(str(SS_JSON_SSIDS));

    JsonArray arr = obj[str(SS_JSON_SSIDS)].as<JsonArray>();

    // for (uint32_t i = 0; i < arr.size() && i < SSID_LIST_SIZE; i++) {
    //     JsonArray& tmpArray = arr.get<JsonVariant>(i);
    //     SSIDS_internal_add(tmpArray.get<String>(0), tmpArray.get<bool>(1), tmpArray.get<int>(2));
    // }

    for(JsonArray v : arr) {
        SSIDS_internal_add(v[0].as<String>(), v[1].as<bool>(), v[2].as<int>());
        Device_sort();
    }

    prnt(SS_LOADED);
    prntln(SSIDS_FILE_PATH);
}

void Accesspoints::SSIDS_load(String filepath) {
    String tmp = SSIDS_FILE_PATH;

    SSIDS_FILE_PATH = filepath;
    SSIDS_load();
    SSIDS_FILE_PATH = tmp;
}

void Accesspoints::SSIDS_removeAll() {
    SSIDS_internal_removeAll();
    prntln(SS_CLEARED);
    SSIDS_changed = true;
}

void Accesspoints::SSIDS_save(bool force) {
    if (!force && !SSIDS_changed) return;

    String buf = String();                              // create buffer

    buf += String(OPEN_CURLY_BRACKET) + String(DOUBLEQUOTES) + str(SS_JSON_RANDOM) + String(DOUBLEQUOTES) + String(
        DOUBLEPOINT) + b2s(SSIDS_randomMode) + String(COMMA); // {"random":false,
    buf += String(DOUBLEQUOTES) + str(SS_JSON_SSIDS) + String(DOUBLEQUOTES) + String(DOUBLEPOINT) +
           String(OPEN_BRACKET);                        // "ssids":[

    if (!file_spiffs::writeFile(SSIDS_FILE_PATH, buf)) {
        prnt(F_ERROR_SAVING);
        prntln(SSIDS_FILE_PATH);
        return;
    }
    buf = String(); // clear buffer

    String name;
    int    c = SSIDS_count();

    for (int i = 0; i < c; i++) {
        name = escape(SSIDS_getName(i));

        buf += String(OPEN_BRACKET) + String(DOUBLEQUOTES) + name + String(DOUBLEQUOTES) + String(COMMA); // ["name",
        buf += b2s(SSIDS_getWPA2(i)) + String(COMMA);                                                           // false,
        buf += String(SSIDS_getLen(i)) + String(CLOSE_BRACKET);                                                 // 12]

        if (i < c - 1) buf += COMMA;                                                                      // ,

        if (buf.length() >= 1024) {
            if (!file_spiffs::appendFile(SSIDS_FILE_PATH, buf)) {
                prnt(F_ERROR_SAVING);
                prntln(SSIDS_FILE_PATH);
                return;
            }

            buf = String(); // clear buffer
        }
    }

    buf += String(CLOSE_BRACKET) + String(CLOSE_CURLY_BRACKET); // ]}

    if (!file_spiffs::appendFile(SSIDS_FILE_PATH, buf)) {
        prnt(F_ERROR_SAVING);
        prntln(SSIDS_FILE_PATH);
        return;
    }

    prnt(SS_SAVED_IN);
    prntln(SSIDS_FILE_PATH);
    SSIDS_changed = false;
}

void Accesspoints::SSIDS_save(bool force, String filepath) {
    String tmp = SSIDS_FILE_PATH;

    SSIDS_FILE_PATH = filepath;
    SSIDS_save(force);
    SSIDS_FILE_PATH = tmp;
}

void Accesspoints::SSIDS_update() {
    if (SSIDS_randomMode) {
        if (currentTime - SSIDS_randomTime > SSIDS_randomInterval * 1000) {
            prntln(SS_RANDOM_INFO);

            for (int i = 0; i < SSID_LIST_SIZE; i++) {
                SSID newSSID;

                if (SSIDS_check(i)) newSSID = list_SSID->get(i);

                newSSID.name = String();
                newSSID.len  = 32;

                for (int i = 0; i < 32; i++) newSSID.name += char(random(32, 127));

                newSSID.wpa2 = random(0, 2);

                if (SSIDS_check(i)) list_SSID->replace(i, newSSID);
                else list_SSID->add(newSSID);
            }

            SSIDS_randomTime = currentTime;
            SSIDS_changed    = true;
        }
    }
}

String Accesspoints::SSIDS_getName(int num) {
    return SSIDS_check(num) ? list_SSID->get(num).name : String();
}

bool Accesspoints::SSIDS_getWPA2(int num) {
    return SSIDS_check(num) ? list_SSID->get(num).wpa2 : false;
}

int Accesspoints::SSIDS_getLen(int num) {
    return SSIDS_check(num) ? list_SSID->get(num).len : 0;
}

void Accesspoints::SSIDS_setWPA2(int num, bool wpa2) {
    SSID newSSID = list_SSID->get(num);

    newSSID.wpa2 = wpa2;
    list_SSID->replace(num, newSSID);
}

String Accesspoints::SSIDS_getEncStr(int num) {
    if (SSIDS_getWPA2(num)) return "WPA2";
    else return "-";
}

void Accesspoints::SSIDS_remove(int num) {
    if (!SSIDS_check(num)) return;

    SSIDS_internal_remove(num);
    prnt(SS_REMOVED);
    prntln(SSIDS_getName(num));
    SSIDS_changed = true;
}

String Accesspoints::SSIDS_randomize(String name) {
    int ssidlen = name.length();

    if (ssidlen > 32) name = name.substring(0, 32);

    if (ssidlen < 32) {
        for (int i = ssidlen; i < 32; i++) {
            int rnd = random(3);

            if ((i < 29) && (rnd == 0)) { // ZERO WIDTH SPACE
                name += char(0xE2);
                name += char(0x80);
                name += char(0x8B);
                i    += 2;
            } else if ((i < 30) && (rnd == 1)) { // NO-BREAK SPACE
                name += char(0xC2);
                name += char(0xA0);
                i    += 1;
            } else {
                name += char(0x20); // SPACE
            }
        }
    }
    return name;
}

void Accesspoints::SSIDS_add(String name, bool wpa2, int clones, bool force) {
    if (list_SSID->size() >= SSID_LIST_SIZE) {
        if (force) {
            SSIDS_internal_remove(0);
        } else {
            prntln(SS_ERROR_FULL);
            return;
        }
    }

    if (clones > SSID_LIST_SIZE) clones = SSID_LIST_SIZE;

    for (int i = 0; i < clones; i++) {
        SSIDS_internal_add(clones > 1 ? SSIDS_randomize(name) : name, wpa2, name.length());

        if (list_SSID->size() > SSID_LIST_SIZE) SSIDS_internal_remove(0);
    }

    prnt(SS_ADDED);
    prntln(name);
    SSIDS_changed = true;
}

void Accesspoints::SSIDS_cloneSelected(bool force) {
    if (selected() > 0) { //accesspoints.selected()
        int clones = SSID_LIST_SIZE;

        if (!force) clones -= list_SSID->size();
        clones /= selected();

        int apCount = count();

        for (int i = 0; i < apCount; i++) {
            if (getSelected(i)) SSIDS_add(getSSID(i), getEnc(i) != 0, clones, force);
        }
    }
}

bool Accesspoints::SSIDS_getRandom() {
    return SSIDS_randomMode;
}

void Accesspoints::SSIDS_replace(int num, String name, bool wpa2) {
    if (!SSIDS_check(num)) return;

    int len = name.length();

    if (len > 32) len = 32;
    SSID newSSID;

    newSSID.name = SSIDS_randomize(name);
    newSSID.wpa2 = wpa2;
    newSSID.len  = (uint8_t)len;
    list_SSID->replace(num, newSSID);

    prnt(SS_REPLACED);
    prntln(name);
    SSIDS_changed = true;
}

void Accesspoints::SSIDS_print(int num) {
    SSIDS_print(num, true, false);
}

void Accesspoints::SSIDS_print(int num, bool header, bool footer) {
    if (!SSIDS_check(num)) return;

    if (header) {
        prntln(SS_TABLE_HEADER);
        prntln(SS_TABLE_DIVIDER);
    }

    prnt(leftRight(String(), (String)num, 2));
    prnt(leftRight(String(SPACE), SSIDS_getEncStr(num), 5));
    prntln(leftRight(String(SPACE) + SSIDS_getName(num), String(), 33));

    if (footer) prntln(SS_TABLE_DIVIDER);
}

void Accesspoints::SSIDS_printAll() {
    prntln(SS_HEADER);
    int c = SSIDS_count();

    if (c == 0) prntln(SS_ERROR_EMPTY);
    else
        for (int i = 0; i < c; i++) print(i, i == 0, i == c - 1);
}

int Accesspoints::SSIDS_count() {
    return list_SSID->size();
}

bool Accesspoints::SSIDS_check(int num) {
    return num >= 0 && num < count();
}

void Accesspoints::SSIDS_enableRandom(uint32_t randomInterval) {
    SSIDS_randomMode            = true;
    SSIDS_randomInterval = randomInterval;
    prntln(SS_RANDOM_ENABLED);
    SSIDS_update();
}

void Accesspoints::SSIDS_disableRandom() {
    SSIDS_randomMode = false;
    SSIDS_internal_removeAll();
    prntln(SS_RANDOM_DISABLED);
}

void Accesspoints::SSIDS_internal_add(String name, bool wpa2, int len) {
    if (len > 32) {
        name = name.substring(0, 32);
        len  = 32;
    }

    name = fixUtf8(name);

    SSID newSSID;

    newSSID.name = name;
    newSSID.wpa2 = wpa2;
    newSSID.len  = (uint8_t)len;

    list_SSID->add(newSSID);
}

void Accesspoints::SSIDS_internal_remove(int num) {
    list_SSID->remove(num);
}

void Accesspoints::SSIDS_internal_removeAll() {
    list_SSID->clear();
}
