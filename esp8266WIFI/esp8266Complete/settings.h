/* This software is licensed under the MIT License: https://github.com/spacehuhntech/esp8266_deauther */

#pragma once

#include <Arduino.h> // Arduino String, Serial uint8_t ...
//#include "A_config.h" //INTERVAL_100MS

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


// ===== SETTINGS ===== //
typedef struct settings_t {
    uint32_t                magic_num;
    version_t               version;
    autosave_settings_t     autosave;
    attack_settings_t       attack;
    wifi_settings_t         wifi;
    sniffer_settings_t      sniffer;
    access_point_settings_t ap;
} settings_t;
namespace settings {
    const attack_settings_t& getAttackSettings();
}