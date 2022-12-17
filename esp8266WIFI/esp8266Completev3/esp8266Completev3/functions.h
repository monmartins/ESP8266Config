#pragma once

#include "Arduino.h"
#include "oui.h" // data_macs


extern "C" {
  #include "user_interface.h" //wifi_set_channel
}

/*
   Here is a collection of useful functions and variables.
   They are used globally via an 'extern' reference in every class.
   Making everything static will lead to problems with the Arduino ESP8266 2.0.0 SDK,
   there were some fixed in later version but we need to use the old version for injecting deauth packets.
 */


//
const char AP_HIDDE_SSID[] PROGMEM = "*HIDDEN*";
const char AP_WEP[] PROGMEM = "WEP";
const char AP_WPA[] PROGMEM = "WPA";
const char AP_WPA2[] PROGMEM = "WPA2";
const char AP_AUTO[] PROGMEM = "WPA*";
const char AP_SELECTED[] PROGMEM = "Selected access point ";
const char AP_DESELECTED[] PROGMEM = "Deselected access point ";
const char AP_REMOVED[] PROGMEM = "Removed access point ";
const char AP_SELECTED_ALL[] PROGMEM = "Selected all APs";
const char AP_DESELECTED_ALL[] PROGMEM = "Deselected all APs";
const char AP_REMOVED_ALL[] PROGMEM = "Removed all APs";
const char AP_NO_AP_ERROR[] PROGMEM = "ERROR: No AP found with ID ";
//
// ===== GLOBAL STRINGS ===== //

// Often used characters, therefor in the RAM
const char CURSOR              = '|';
const char SPACE               = ' ';
const char DOUBLEPOINT         = ':';
const char EQUALS              = '=';
const char HASHSIGN            = '#';
const char ASTERIX             = '*';
const char PERCENT             = '%';
const char DASH                = '-';
const char QUESTIONMARK        = '?';
const char ZERO                = '0';
const char S                   = 's';
const char M                   = 'm';
const char D                   = 'd';
const char DOUBLEQUOTES        = '\"';
const char SLASH               = '/';
const char NEWLINE             = '\n';
const char CARRIAGERETURN      = '\r';
const char SEMICOLON           = ';';
const char BACKSLASH           = '\\';
const char POINT               = '.';
const char VERTICALBAR         = '|';
const char COMMA               = ',';
const char ENDOFLINE           = '\0';
const char OPEN_BRACKET        = '[';
const char CLOSE_BRACKET       = ']';
const char OPEN_CURLY_BRACKET  = '{';
const char CLOSE_CURLY_BRACKET = '}';
//
const char STR_TRUE[] PROGMEM = "true";
const char STR_FALSE[] PROGMEM = "false";
const char STR_MIN[] PROGMEM = "min";
//
// ===== FUNCTIONS ===== //
const char F_ERROR_MAC[] PROGMEM = "ERROR: MAC address invalid";
const char F_COLOR_INVALID[] PROGMEM = "ERROR: Color code invalid";
const char F_ERROR_READING_FILE[] PROGMEM = "ERROR: reading file ";
const char F_LINE[] PROGMEM = "[%d] ";
const char F_ERROR_FILE[] PROGMEM = "ERROR: File doesn't exist ";
const char F_ERROR_OPEN[] PROGMEM = "ERROR couldn't open ";
const char F_ERROR_PARSING_JSON[] PROGMEM = "ERROR parsing JSON ";
const char F_ERROR_TO_BIG[] PROGMEM = "ERROR file too big ";
const char F_TMP[] PROGMEM = "/tmp";
const char F_COPY[] PROGMEM = "_copy";
const char F_ERROR_SAVING[] PROGMEM = "ERROR: saving file. Try 'format' and restart - ";


const char ST_TABLE_DIVIDER[] PROGMEM = "==============================================================================================================";
const char ST_SMALLER_ONESEC[] PROGMEM = "<1sec";
const char ST_SMALLER_ONEMIN[] PROGMEM = "<1min";
const char ST_BIGER_ONEHOUR[] PROGMEM = ">1h";
const char ST_SELECTED_STATION[] PROGMEM = "Selected station ";
const char ST_DESELECTED_STATION[] PROGMEM = "Deselected station ";
const char ST_ERROR_ID[] PROGMEM = "ERROR: No station found with ID ";
const char ST_SELECTED_ALL[] PROGMEM = "Selected all stations";
const char ST_DESELECTED_ALL[] PROGMEM  = "Deselected all stations";

const char ST_CLEARED_LIST[] PROGMEM = "Cleared station list";
const char ST_REMOVED_STATION[] PROGMEM = "Removed station ";
const char ST_LIST_EMPTY[] PROGMEM = "Station list is empty :(";
const char ST_HEADER[] PROGMEM = "[===== Stations =====]";
const char ST_NO_DEVICES_SELECTED[] PROGMEM = "No devices selected";
const char ST_TABLE_HEADER[] PROGMEM = "ID MAC               Ch Name             Vendor   Pkts     AP                               Last Seen Selected";

// ===== ACCESS POINTS ===== //
const char AP_HEADER[] PROGMEM = "[===== Access Points =====]";
const char AP_LIST_EMPTY[] PROGMEM = "AP list is empty :(";
const char AP_NO_AP_SELECTED[] PROGMEM = "No APs selected";
const char AP_TABLE_HEADER[] PROGMEM = "ID SSID                             Name             Ch RSSI Enc. Mac               Vendor   Selected";
const char AP_TABLE_DIVIDER[] PROGMEM = "=====================================================================================================";

// ===== NAMES ===== //
const char N_SAVED[] PROGMEM = "Device names saved in ";
const char N_LOADED[] PROGMEM = "Device names loaded from ";
const char N_REMOVED_ALL[] PROGMEM = "Removed all saved device names";
const char N_HEADER[] PROGMEM = "[===== Saved Devices =====]";
const char N_TABLE_HEADER[] PROGMEM =  "ID MAC               Vendor   Name             AP-BSSID          Ch Selected";
const char N_TABLE_DIVIDER[] PROGMEM = "============================================================================";
const char N_ERROR_LIST_EMPTY[] PROGMEM = "Device name list is empty :(";
const char N_ERROR_NO_SELECTED[] PROGMEM = "No devices selected";
const char N_ERROR_LIST_FULL[] PROGMEM = "ERROR: Name list is full!";
const char N_ADDED[] PROGMEM = "Added to device name list ";
const char N_REPLACED[] PROGMEM = "Replaced device name list ";
const char N_REMOVED[] PROGMEM = "Removed from the device name list ";
const char N_CHANGED_NAME[] PROGMEM = "Changed device name";
const char N_CHANGED_MAC[] PROGMEM = "Changed device mac";
const char N_CHANGED_BSSID[] PROGMEM = "Changed device AP-BSSID";
const char N_CHANGED_CH[] PROGMEM = "Changed device channel";
const char N_SELECTED[] PROGMEM = "Selected device ";
const char N_ERROR_NOT_FOUND[] PROGMEM = "No device found with name ";
const char N_DESELECTED[] PROGMEM = "Deselected device ";
const char N_SELECTED_ALL[] PROGMEM = "Selected all device names";
const char N_DESELECTED_ALL[] PROGMEM = "Deselected all device names";

// ===== SCAN ==== //
const char SC_START_CLIENT[] PROGMEM = "Starting Scan for stations (client devices) - ";
const char SS_START_SNIFFER[] PROGMEM = "Starting packet sniffer - ";
const char SC_ERROR_NO_AP[] PROGMEM =
    "ERROR: AP-list empty! Can't scan for clients, please Scan for Accesspoints first.";
const char SC_INFINITELY[] PROGMEM = " infinitely";
const char SC_ON_CHANNEL[] PROGMEM = " on channel ";
const char SC_START_AP[] PROGMEM = "Starting scan for access points (Wi-Fi networks)...";
const char SC_ONE_TO[] PROGMEM = "1 - ";
const char SC_STOPPED[] PROGMEM = "Stopped scan";
const char SC_RESTRAT[] PROGMEM = "Scan will restart in ";
const char SC_CONTINUE[] PROGMEM = "s - type stop to disable the continuous mode";
const char SC_RESTART[] PROGMEM = "restarting in ";
const char SC_ERROR_MODE[] PROGMEM = "ERROR: Invalid scan mode ";
const char SC_OUTPUT_A[] PROGMEM = "Scanning WiFi [%+2u%%]: %+3u packets/s | %+2u devices | %+2u deauths\r\n";
const char SC_OUTPUT_B[] PROGMEM = "Scanning WiFi: %+3u packets/s | %+2u devices | %+2u deauths\r\n";
const char SC_JSON_APS[] PROGMEM = "aps";
const char SC_JSON_STATIONS[] PROGMEM = "stations";
const char SC_JSON_NAMES[] PROGMEM = "names";
const char SC_SAVED[] PROGMEM = "Saved scan results";
const char SC_SAVED_IN[] PROGMEM = "Scan results saved in ";
const char SC_MODE_OFF[] PROGMEM = "-";
const char SC_MODE_AP[] PROGMEM = "APs";
const char SC_MODE_ST[] PROGMEM = "STs";
const char SC_MODE_ALL[] PROGMEM = "AP+ST";
const char SC_MODE_SNIFFER[] PROGMEM = "Sniffer";

// ===== SETTINGS ====== //
// Version
const char S_JSON_VERSION[] PROGMEM = "version";

// Autosave
const char S_JSON_AUTOSAVE[] PROGMEM = "autosave";
const char S_JSON_AUTOSAVETIME[] PROGMEM = "autosavetime";

// Attack
const char S_JSON_BEACONCHANNEL[] PROGMEM = "beaconchannel";
const char S_JSON_RANDOMTX[] PROGMEM = "randomTX";
const char S_JSON_ATTACKTIMEOUT[] PROGMEM = "attacktimeout";
const char S_JSON_DEAUTHSPERTARGET[] PROGMEM = "deauthspertarget";
const char S_JSON_DEAUTHREASON[] PROGMEM = "deauthReason";
const char S_JSON_BEACONINTERVAL[] PROGMEM = "beaconInterval";
const char S_JSON_PROBESPERSSID[] PROGMEM = "probesPerSSID";

// WiFi
const char S_JSON_CHANNEL[] PROGMEM = "channel";
const char S_JSON_MACST[] PROGMEM = "macSt";
const char S_JSON_MACAP[] PROGMEM = "macAP";

// Sniffer
const char S_JSON_CHTIME[] PROGMEM = "chtime";
const char S_JSON_MIN_DEAUTHS[] PROGMEM = "minDeauths";

// AP
const char S_JSON_SSID[] PROGMEM = "ssid";
const char S_JSON_PASSWORD[] PROGMEM = "password";
const char S_JSON_HIDDEN[] PROGMEM = "hidden";
const char S_JSON_IP[] PROGMEM = "ip";

// Web
const char S_JSON_WEBINTERFACE[] PROGMEM = "webinterface";
const char S_JSON_CAPTIVEPORTAL[] PROGMEM = "captivePortal";
const char S_JSON_WEB_SPIFFS[] PROGMEM = "webSpiffs";
const char S_JSON_LANG[] PROGMEM = "lang";

#define debug_init()\
    DEBUG_PORT.begin(DEBUG_BAUD);\
    DEBUG_PORT.setTimeout(LONG_MAX);\
    DEBUG_PORT.println();

#define debug(...) DEBUG_PORT.print(__VA_ARGS__)
#define debugln(...) DEBUG_PORT.println(__VA_ARGS__)
#define debugf(...) DEBUG_PORT.printf(__VA_ARGS__)
#define debugF(...) DEBUG_PORT.print(F(__VA_ARGS__))
#define debuglnF(...) DEBUG_PORT.println(F(__VA_ARGS__))

// namespace functions{

// uint8_t broadcast[6];// = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// uint8_t wifi_channel;// = 1;
// }


// ===== UTF8 FIX ===== //
String escape(String str);

bool ascii(char c);
bool printableAscii(char c);
bool getBit(uint8_t b, uint8_t n);
uint8_t utf8(uint8_t c);

bool utf8Part(uint8_t c);

String fixUtf8(String str);

String removeUtf8(String str);

int utf8Len(String str);

String replaceUtf8(String str, String r);

// ===== LANGUAGE STRING FUNCTIONS ===== //

// for reading Strings from the PROGMEM
String str(const char* ptr);

// for converting keywords
String keyword(const char* keywordPtr);

// equals function
bool eqls(const char* str, const char* keywordPtr);

bool eqls(String str, const char* keywordPtr);
// boolean to string
String b2s(bool input);

// boolean to asterix *
String b2a(bool input);

// string to boolean
bool s2b(String input);

// ===== PRINT FUNCTIONS ===== //
void prnt(const String s);

void prnt(const bool b);

void prnt(const char c);

void prnt(const char* ptr);

void prnt(const char* ptr, int len);

void prnt(const int i);

void prnt(const uint32_t i);

void prntln();

void prntln(const String s);

void prntln(const bool b);

void prntln(const char c);

void prntln(const char* ptr);

void prntln(const char* ptr, int len);

void prntln(const int i);

void prntln(const uint32_t i);

/* ===== WiFi ===== */
void setWifiChannel(uint8_t ch, bool force);

void setOutputPower(float dBm);

/* ===== MAC ADDRESSES ===== */
bool macBroadcast(uint8_t* mac);

bool macValid(uint8_t* mac);

bool macMulticast(uint8_t* mac);

/* ===== VENDOR LIST (oui.h) ===== */
void getRandomMac(uint8_t* mac);

int binSearchVendors(uint8_t* searchBytes, int lowerEnd, int upperEnd);

String searchVendor(uint8_t* mac);

/* ===== STRING ===== */
String bytesToStr(const uint8_t* b, uint32_t size);

String macToStr(const uint8_t* mac);

bool strToMac(String macStr, uint8_t* mac);

bool strToIP(String ipStr, uint8_t* ip);

void strToColor(String str, uint8_t* buf);

String center(String a, int len);

String left(String a, int len);

String right(String a, int len);

String leftRight(String a, String b, int len);

// /* ===== SPIFFS ===== */
// bool progmemToSpiffs(const char* adr, int len, String path) {
//     prnt(str(SETUP_COPYING) + path + str(SETUP_PROGMEM_TO_SPIFFS));
//     File f = LittleFS.open(path, "w+");

//     if (!f) {
//         prntln(SETUP_ERROR);
//         return false;
//     }

//     for (int i = 0; i < len; i++) {
//         f.write(pgm_read_byte_near(adr + i));
//     }
//     f.close();

//     prntln(SETUP_OK);

//     return true;
// }

// bool readFile(String path, String& buf) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;
//     File f = LittleFS.open(path, "r");

//     if (!f) return false;

//     if (f.size() == 0) return false;

//     while (f.available()) buf += (char)f.read();

//     f.close();

//     return true;
// }

// void readFileToSerial(String path, bool showLineNum) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;
//     File f = LittleFS.open(path, "r");

//     if (!f) {
//         prnt(F_ERROR_READING_FILE);
//         prntln(path);
//         return;
//     }

//     uint32_t c = 0;
//     char     tmp;

//     if (showLineNum) {
//         prnt(leftRight(String(), (String)c + String(VERTICALBAR), 6));
//     }

//     while (f.available()) {
//         tmp = f.read();
//         prnt(tmp);

//         if ((tmp == NEWLINE) && showLineNum) {
//             c++;
//             prnt(leftRight(String(), (String)c + String(VERTICALBAR), 6));
//         }
//     }

//     f.close();
// }

// bool copyFile(String pathFrom, String pathTo) {
//     if (pathFrom.charAt(0) != SLASH) pathFrom = String(SLASH) + pathFrom;

//     if (pathTo.charAt(0) != SLASH) pathTo = String(SLASH) + pathTo;

//     if (!LittleFS.exists(pathFrom)) {
//         prnt(F_ERROR_FILE);
//         prntln(pathFrom);
//         return false;
//     }

//     File f1 = LittleFS.open(pathFrom, "r");
//     File f2 = LittleFS.open(pathTo, "w+");

//     if (!f1 || !f2) return false;

//     while (f1.available()) {
//         f2.write(f1.read());
//     }

//     return true;
// }

// bool renameFile(String pathFrom, String pathTo) {
//     if (pathFrom.charAt(0) != SLASH) pathFrom = String(SLASH) + pathFrom;

//     if (pathTo.charAt(0) != SLASH) pathTo = String(SLASH) + pathTo;

//     if (!LittleFS.exists(pathFrom)) {
//         prnt(F_ERROR_FILE);
//         prntln(pathFrom);
//         return false;
//     }

//     LittleFS.rename(pathFrom, pathTo);
//     return true;
// }

// bool writeFile(String path, String& buf) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;
//     File f = LittleFS.open(path, "w+");

//     if (!f) return false;

//     uint32_t len = buf.length();

//     for (uint32_t i = 0; i < len; i++) f.write(buf.charAt(i));
//     f.close();

//     return true;
// }

// bool appendFile(String path, String& buf) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;
//     File f = LittleFS.open(path, "a+");

//     if (!f) return false;

//     uint32_t len = buf.length();

//     for (uint32_t i = 0; i < len; i++) f.write(buf[i]);
//     f.close();

//     return true;
// }

// void checkFile(String path, String data) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     if (!LittleFS.exists(path)) writeFile(path, data);
// }

// bool removeLines(String path, int lineFrom, int lineTo) {
//     int  c = 0;
//     char tmp;

//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     String tmpPath = str(F_TMP) + path + str(F_COPY);

//     File f  = LittleFS.open(path, "r");
//     File f2 = LittleFS.open(tmpPath, "w");

//     if (!f || !f2) return false;

//     while (f.available()) {
//         tmp = f.read();

//         if ((c < lineFrom) || (c > lineTo)) f2.write(tmp);

//         if (tmp == NEWLINE) c++;
//     }

//     f.close();
//     f2.close();
//     LittleFS.remove(path);
//     LittleFS.rename(tmpPath, path);

//     return true;
// }

// bool replaceLine(String path, int line, String& buf) {
//     int  c = 0;
//     char tmp;

//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     String tmpPath = "/tmp" + path + "_copy";

//     File f  = LittleFS.open(path, "r");
//     File f2 = LittleFS.open(tmpPath, "w");

//     if (!f || !f2) return false;

//     while (f.available()) {
//         tmp = f.read();

//         if (c != line) f2.write(tmp);
//         else {
//             f2.println(buf);

//             while (f.read() != NEWLINE && f.available()) {}
//             c++;
//         }

//         if (tmp == NEWLINE) c++;
//     }

//     f.close();
//     f2.close();
//     LittleFS.remove(path);
//     LittleFS.rename(tmpPath, path);

//     return true;
// }

// JsonVariant parseJSONFile(String path, DynamicJsonBuffer& jsonBuffer) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     // create JSON Variant
//     JsonVariant root;

//     // create buffer
//     String buf = "";

//     // read file into buffer
//     if (!readFile(path, buf)) { // if file couldn't be opened, send 404 error
//         prnt(F_ERROR_OPEN);
//         prntln(path);
//         buf = "{}";
//     }

//     // parse file-buffer into a JSON Variant
//     root = jsonBuffer.parse(buf);

//     // if parsing unsuccessful
//     if (!root.success()) {
//         prnt(F_ERROR_PARSING_JSON);
//         prntln(path);
//         prntln(buf);
//     }

//     return root;
// }

// bool removeFile(String path) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;
//     return LittleFS.remove(path);
// }

// void saveJSONFile(String path, JsonObject& root) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     // create buffer
//     String buf;

//     // convert JSON object into string and write it into buffer
//     root.printTo(buf);

//     // if buffer too big
//     if (buf.length() > 2048) {
//         prntln(F_ERROR_TO_BIG);
//         prntln(path);
//         prntln(buf);
//         return;
//     }

//     // write buffer into SPIFFS file
//     writeFile(path, buf);
// }

// void saveJSONFile(String path, JsonArray& root) {
//     if (path.charAt(0) != SLASH) path = String(SLASH) + path;

//     // create buffer
//     String buf;

//     // convert JSON object into string and write it into buffer
//     root.printTo(buf);

//     // if buffer too big
//     if (buf.length() > 2048) {
//         prntln(F_ERROR_TO_BIG);
//         prntln(path);
//         prntln(buf);
//         return;
//     }

//     // write buffer into SPIFFS file
//     writeFile(path, buf);
// }

String formatBytes(size_t bytes);