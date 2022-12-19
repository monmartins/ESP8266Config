#pragma once
#include "Arduino.h"

#define ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200

// ======== CONSTANTS ========== //
// Do not change these values unless you know what you're doing!
#define DEAUTHER_VERSION "1.0.0"
#define DEAUTHER_VERSION_MAJOR 1
#define DEAUTHER_VERSION_MINOR 0
#define DEAUTHER_VERSION_REVISION 0

// ===== Web ===== //
#ifndef WEB_IP_ADDR
  #define WEB_IP_ADDR (192, 168, 4, 1)
  #define GATEWAY (192, 168, 4, 1)
  #define SUBNET (255,255,255,0)
#endif // ifndef WEB_IP_ADDR

#define MAGIC_NUM 3416245

#define JSON_FLAG(_NAME, _VALUE)\
    str += String('"') + String(FPSTR(_NAME)) + String(F("\":")) + String(_VALUE ? "true" : "false") + String(',');

#define JSON_VALUE(_NAME, _VALUE)\
    str += String('"') + String(FPSTR(_NAME)) + String(F("\":\"")) + String(_VALUE) + String(F("\","));

#define JSON_INT(_NAME, _VALUE)\
    str += String('"') + String(FPSTR(_NAME)) + String(F("\":")) + String(_VALUE) + String(',');

#define JSON_HEX(_NAME, _BYTES, _LEN)\
    str += String('"') + String(FPSTR(_NAME)) + String(F("\":\""));\
    for (int i = 0; i<_LEN; i++) {\
        if (i > 0) str            += ':';\
        if (_BYTES[i] < 0x10) str += '0';\
        str                       += String(_BYTES[i], HEX);\
    }\
    str += String(F("\","));

#define JSON_DEC(_NAME, _BYTES, _LEN)\
    str += String(F("\"")) + String(FPSTR(_NAME)) + String(F("\":\""));\
    for (int i = 0; i<_LEN; i++) {\
        if (i > 0) str += '.';\
        str            += String(_BYTES[i]);\
    }\
    str += String(F("\","));

// ===== AUTOSAVE ===== //
#ifndef AUTOSAVE_ENABLED
  #define AUTOSAVE_ENABLED true
#endif /* ifndef ATTACK_ALL_CH */

#ifndef AUTOSAVE_TIME
  #define AUTOSAVE_TIME 60
#endif /* ifndef ATTACK_ALL_CH */

// ===== ATTACK ===== //
#ifndef ATTACK_ALL_CH
  #define ATTACK_ALL_CH false
#endif /* ifndef ATTACK_ALL_CH */

#ifndef RANDOM_TX
  #define RANDOM_TX false
#endif /* ifndef RANDOM_TX */

#ifndef ATTACK_TIMEOUT
  #define ATTACK_TIMEOUT 600
#endif /* ifndef ATTACK_TIMEOUT */

#ifndef DEAUTHS_PER_TARGET
  #define DEAUTHS_PER_TARGET 25
#endif /* ifndef DEAUTHS_PER_TARGET */

#ifndef DEAUTH_REASON
  #define DEAUTH_REASON 1
#endif /* ifndef DEAUTH_REASON */

#ifndef BEACON_INTERVAL_100MS
  #define BEACON_INTERVAL_100MS true
#endif /* ifndef BEACON_INTERVAL_100MS */

#ifndef PROBE_FRAMES_PER_SSID
  #define PROBE_FRAMES_PER_SSID 1
#endif /* ifndef PROBE_FRAMES_PER_SSID */

// ===== SNIFFER ===== //
#ifndef CH_TIME
  #define CH_TIME 200
#endif /* ifndef CH_TIME */

#ifndef MIN_DEAUTH_FRAMES
  #define MIN_DEAUTH_FRAMES 3
#endif /* ifndef MIN_DEAUTH_FRAMES */

// ===== ACCESS POINT ===== //
#ifndef AP_SSID
  #define AP_SSID "pwned"
#endif /* ifndef AP_SSID */

#ifndef AP_PASSWD
  #define AP_PASSWD "deauther"
#endif /* ifndef AP_PASSWD */

#ifndef AP_HIDDEN
  #define AP_HIDDEN false
#endif /* ifndef AP_HIDDEN */

#ifndef AP_IP_ADDR
  #define AP_IP_ADDR { 192, 168, 4, 1 }
#endif /* ifndef AP_IP_ADDR */

// ===== WEB INTERFACE ===== //
#ifndef WEB_ENABLED
  #define WEB_ENABLED true
#endif /* ifndef WEB_ENABLED */

#ifndef WEB_CAPTIVE_PORTAL
  #define WEB_CAPTIVE_PORTAL false
#endif /* ifndef WEB_CAPTIVE_PORTAL */

#ifndef WEB_USE_SPIFFS
  #define WEB_USE_SPIFFS false
#endif /* ifndef WEB_USE_SPIFFS */

#ifndef DEFAULT_LANG
  #define DEFAULT_LANG "en"
#endif /* ifndef DEFAULT_LANG */

// ===== CLI ===== //
#ifndef CLI_ENABLED
  #define CLI_ENABLED true
#endif /* ifndef CLI_ENABLED */

#ifndef CLI_ECHO
  #define CLI_ECHO true
#endif /* ifndef CLI_ECHO */

// ===== VERSION ===== //
typedef struct version_t {
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
} version_t;

// ===== AUTOSAVE ===== //
typedef struct autosave_settings_t {
    bool     enabled;
    uint32_t time;
} autosave_t;

// ===== ATTACK ===== //
typedef enum beacon_interval_t {
    INTERVAL_1S    = 0,
    INTERVAL_100MS = 1
} beacon_interval_t;

typedef struct attack_settings_t {
    // General
    bool     attack_all_ch;
    bool     random_tx;
    uint32_t timeout;

    // Deauth
    uint8_t deauths_per_target;
    uint8_t deauth_reason;

    // Beacon
    beacon_interval_t beacon_interval;

    // Probe
    uint8_t probe_frames_per_ssid;
} attack_settings_t;

// ====== WIFI ====== //
typedef struct wifi_settings_t {
    uint8_t channel;
    uint8_t mac_st[6];
    uint8_t mac_ap[6];
} wifi_settings_t;

// ===== SNIFFER ===== //
typedef struct sniffer_settings_t {
    uint16_t channel_time;
    uint16_t min_deauth_frames;
} sniffer_settings_t;

// ===== ACCESS POINT ===== //
typedef struct access_point_settings_t {
    char    ssid[33];
    char    password[65];
    bool    hidden;
    uint8_t ip[4];
} access_point_settings_t;

// ===== WEB INTERFACE ===== //
typedef struct web_settings_t {
    bool enabled;
    bool captive_portal;
    bool use_spiffs;
    char lang[3];
} web_settings_t;

// ===== SETTINGS ===== //
typedef struct settings_t {
    uint32_t                magic_num;
    version_t               version;
    autosave_settings_t     autosave;
    attack_settings_t       attack;
    wifi_settings_t         wifi;
    sniffer_settings_t      sniffer;
    access_point_settings_t ap;
    web_settings_t          web;
} settings_t;

namespace config {
    void save(bool force = false);
    void saveTo(String cnt, String path);
    void load();
    void load(String path);

    void reset();
    void print();

    const settings_t& getAllSettings();
    const version_t& getVersion();
    const autosave_settings_t& getAutosaveSettings();
    const attack_settings_t& getAttackSettings();
    const wifi_settings_t& getWifiSettings();
    const sniffer_settings_t& getSnifferSettings();
    const access_point_settings_t& getAccessPointSettings();
    const web_settings_t& getWebSettings();

    void setAllSettings(settings_t& settings);
    void setAutosaveSettings(const autosave_settings_t& autosave);
    void setAttackSettings(const attack_settings_t& attack);
    void setWifiSettings(const wifi_settings_t& wifi);
    void setSnifferSettings(const sniffer_settings_t& sniffer);
    void setAccessPointSettings(const access_point_settings_t& ap);
    void setWebSettings(const web_settings_t& web);

}