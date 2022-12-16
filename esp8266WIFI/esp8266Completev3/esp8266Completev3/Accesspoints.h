#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}
#include "functions.h"
#include "SimpleList.h"
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
        
        

        void Stations_add(uint8_t* mac, int accesspointNum);
        bool Stations_check(int num);
        
        bool Stations_changed    = false;

        //Device
        int Names_findID(uint8_t* mac);
        String Names_find(uint8_t* mac);
        String Names_getName(int num);
        bool Names_check(int num);
        bool Names_internal_check(int num);
        int Names_count(); 
        bool Device_changed     = false;

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
        String FILE_PATH = "/devices.json";

        SimpleList<AP>* list;
        SimpleList<Station>* list_Station;
        SimpleList<Device>* list_Device;

        bool internal_check(int num);
        void internal_select(int num);
        void internal_deselect(int num);
        void internal_remove(int num);
};