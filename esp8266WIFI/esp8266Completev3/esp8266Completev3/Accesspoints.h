#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}
#include "functions.h"
#include "SimpleList.h"
#include <ArduinoJson.h>
// #include "Names.h"

// extern Names names;

//extern String searchVendor(uint8_t* mac); //functions
// extern String leftRight(String a, String b, int len);
// extern String fixUtf8(String str);
// extern String bytesToStr(const uint8_t* b, uint32_t size);

extern uint8_t  wifi_channel;
extern uint32_t currentTime;

struct AP {
    uint8_t id;
    bool    selected;
};
#define STATION_LIST_SIZE 60
#define DEVICE_LIST_SIZE 25
#define DEVICE_MAX_LENGTH 17
#define SSID_LIST_SIZE 60
class Accesspoints {
    public:
        Accesspoints();

        void sort();
        void sortAfterChannel();

        void add(uint8_t id, bool selected);

        void print(int num);
        void print(int num, bool header, bool footer);

        void select(int num);
        void deselect(int num);
        void remove(int num);
        void select(String ssid);
        void deselect(String ssid);
        void remove(String ssid);

        void printAll();
        void printSelected();
        void selectAll();
        void deselectAll();
        void removeAll();

        String getSSID(int num);
        String getNameStr(int num);
        String getEncStr(int num);
        String getMacStr(int num);
        String getVendorStr(int num);
        String getSelectedStr(int num);
        uint8_t getCh(int num);
        uint8_t getEnc(int num);
        uint8_t getID(int num);
        int getRSSI(int num);
        uint8_t* getMac(int num);
        bool getHidden(int num);
        bool getSelected(int num);

        int find(uint8_t id);

        int count();
        int selected();

        bool check(int num);
        bool changed = false;

        //Station

        void Stations_sort();
        void Stations_sortAfterChannel();

        int Stations_findStation(uint8_t* mac);


        int Stations_count();
        int Stations_selected();

        void Stations_select(int num);
        void Stations_deselect(int num);
        void Stations_remove(int num);

        void Stations_select(String ssid);
        void Stations_deselect(String ssid);
        void Stations_remove(String ssid);

        void Stations_selectAll();
        void Stations_deselectAll();
        void Stations_removeAll();
        void Stations_removeOldest();

        String Stations_getNameStr(int num);
        String Stations_getAPStr(int num);
        String Stations_getMacStr(int num);
        String Stations_getMacVendorStr(int num);
        String Stations_getVendorStr(int num);
        String Stations_getTimeStr(int num);
        String Stations_getSelectedStr(int num);
        uint8_t* Stations_getAPMac(int num);
        String Stations_getAPMacStr(int num);
        uint8_t* Stations_getMac(int num);
        uint32_t* Stations_getPkts(int num);
        uint32_t* Stations_getTime(int num);
        uint8_t Stations_getCh(int num);
        int Stations_getAP(int num);
        bool Stations_getSelected(int num);
        bool Stations_hasName(int num);
        
        void Stations_print(int num);
        void Stations_print(int num, bool header, bool footer);
        void Stations_printAll();
        void Stations_printSelected();
        
        
        void Stations_add(uint8_t* mac, int accesspointNum);
        bool Stations_check(int num);
        
        bool Stations_changed    = false;

        //Device
        int Device_findID(uint8_t* mac);
        String Device_find(uint8_t* mac);

        void Device_load();
        void Device_load(String filepath);
        void Device_save(bool force);
        void Device_save(bool force, String filepath);
        void Device_sort();

        void Device_print(int num);
        void Device_print(int num, bool header, bool footer);
        void Device_select(int num);
        void Device_select(String name);
        void Device_deselect(int num);
        void Device_deselect(String name);
        void Device_add(uint8_t* mac, String name, uint8_t* bssid, uint8_t ch, bool selected, bool force);
        void Device_add(String macStr, String name, String bssidStr, uint8_t ch, bool selected, bool force);
        void Device_replace(int num, String macStr, String name, String bssidStr, uint8_t ch, bool selected);
        void Device_remove(int num);

        void Device_printAll();
        void Device_printSelected();
        void Device_selectAll();
        void Device_deselectAll();
        void Device_removeAll();

        uint8_t* Device_getMac(int num);
        uint8_t* Device_getBssid(int num);
        String Device_getMacStr(int num);
        String Device_getBssidStr(int num);
        String Device_getName(int num);
        String Device_getVendorStr(int num);
        String Device_getSelectedStr(int num);
        uint8_t Device_getCh(int num);
        bool Device_getSelected(int num);
        bool Device_isStation(int num);

        void Device_setName(int num, String name);
        void Device_setMac(int num, String macStr);
        void Device_setCh(int num, uint8_t ch);
        void Device_setBSSID(int num, String bssidStr);

        int Device_count();
        int Device_selected();
        int Device_stations();

        bool Device_check(int num);


        bool Device_force     = false;
        bool Device_changed     = false;

        //SSID
        void SSIDS_load();
        void SSIDS_load(String filepath);
        void SSIDS_save(bool force);
        void SSIDS_save(bool force, String filepath);
        void SSIDS_update();

        void SSIDS_print(int num);
        void SSIDS_print(int num, bool header, bool footer);
        void SSIDS_add(String name, bool wpa2, int clones, bool force);
        void SSIDS_cloneSelected(bool force);
        void SSIDS_remove(int num);
        void SSIDS_enableRandom(uint32_t randomInterval);
        void SSIDS_disableRandom();
        bool SSIDS_getRandom();

        String SSIDS_getName(int num);
        bool SSIDS_getWPA2(int num);
        String SSIDS_getEncStr(int num);
        int SSIDS_getLen(int num);

        void SSIDS_setWPA2(int num, bool wpa2);
        void SSIDS_replace(int num, String name, bool wpa2);

        void SSIDS_printAll();
        void SSIDS_removeAll();

        int SSIDS_count();

    private:
        struct Station {
            uint8_t   ap;   //id of AP
            uint8_t   ch;   //ch of Station
            uint8_t * mac;  //mac address of Station
            uint32_t* pkts;
            uint32_t* time;
            bool      selected;
        };
        bool Stations_internal_check(int num);
        void Stations_internal_select(int num);
        void Stations_internal_deselect(int num);
        void Stations_internal_add(uint8_t* mac, int accesspointNum);
        void Stations_internal_remove(int num);
        void Stations_internal_removeAll();

        struct Device {
            uint8_t* mac;      // mac address
            char   * name;     // name of saved device
            uint8_t* apBssid;  // mac address of AP (if saved device is a station)
            uint8_t  ch;       // Wi-Fi channel of Device
            bool     selected; // select for attacking
        };
        String DEVICE_FILE_PATH = "/devices.json";
        int Device_binSearch(uint8_t* searchBytes, int lowerEnd, int upperEnd);
        bool Device_internal_check(int num);
        void Device_internal_select(int num);
        void Device_internal_deselect(int num);
        void Device_internal_add(uint8_t* mac, String name, uint8_t* bssid, uint8_t ch, bool selected);
        void Device_internal_add(String macStr, String name, String bssidStr, uint8_t ch, bool selected);
        void Device_internal_remove(int num);
        void Device_internal_removeAll();
        //
        // SSIDS
        bool SSIDS_changed            = false;
        bool SSIDS_randomMode         = false;
        uint32_t SSIDS_randomInterval = 2000;
        uint32_t SSIDS_randomTime     = 0;

        struct SSID {
            String  name; // SSID
            bool    wpa2; // WPA2 encrypted or not
            uint8_t len;  // original length (before editing it to be 32 characters)
        };

        String SSIDS_FILE_PATH = "/ssids.json";


        bool SSIDS_check(int num);
        String SSIDS_randomize(String name);

        void SSIDS_internal_add(String name, bool wpa2, int add);
        void SSIDS_internal_remove(int num);
        void SSIDS_internal_removeAll();
        //

        SimpleList<AP>* list;
        SimpleList<Station>* list_Station;
        SimpleList<Device>* list_Device;
        SimpleList<SSID>* list_SSID;

        bool internal_check(int num);
        void internal_select(int num);
        void internal_deselect(int num);
        void internal_remove(int num);
};