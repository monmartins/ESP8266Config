#include "config.h"
#include "functions.h"
#include "file.h"


namespace config {
        // ========== PRIVATE ========== //
    const char* SETTINGS_PATH = "/settings.json";

    settings_t data;
    bool changed = false;
    void get_json(String& str) {
        str = String();
        str.reserve(600);

        str += '{';

        // Version
        JSON_VALUE(S_JSON_VERSION, DEAUTHER_VERSION);

        // Autosave
        JSON_FLAG(S_JSON_AUTOSAVE, data.autosave.enabled);
        JSON_INT(S_JSON_AUTOSAVETIME, data.autosave.time);

        // Attack
        JSON_FLAG(S_JSON_BEACONCHANNEL, data.attack.attack_all_ch);
        JSON_FLAG(S_JSON_RANDOMTX, data.attack.random_tx);
        JSON_INT(S_JSON_ATTACKTIMEOUT, data.attack.timeout);
        JSON_INT(S_JSON_DEAUTHSPERTARGET, data.attack.deauths_per_target);
        JSON_INT(S_JSON_DEAUTHREASON, data.attack.deauth_reason);
        JSON_FLAG(S_JSON_BEACONINTERVAL, data.attack.beacon_interval == INTERVAL_1S);
        JSON_INT(S_JSON_PROBESPERSSID, data.attack.probe_frames_per_ssid);

        // WiFi
        JSON_INT(S_JSON_CHANNEL, data.wifi.channel);
        JSON_HEX(S_JSON_MACST, data.wifi.mac_st, 6);
        JSON_HEX(S_JSON_MACAP, data.wifi.mac_ap, 6);

        // Sniffer
        JSON_INT(S_JSON_CHTIME, data.sniffer.channel_time);
        JSON_INT(S_JSON_MIN_DEAUTHS, data.sniffer.min_deauth_frames);

        // Access Point
        JSON_VALUE(S_JSON_SSID, data.ap.ssid);
        JSON_VALUE(S_JSON_PASSWORD, data.ap.password);
        JSON_FLAG(S_JSON_HIDDEN, data.ap.hidden);
        JSON_DEC(S_JSON_IP, data.ap.ip, 4);

        // Web Interface
        JSON_FLAG(S_JSON_WEBINTERFACE, data.web.enabled);
        JSON_FLAG(S_JSON_CAPTIVEPORTAL, data.web.captive_portal);
        JSON_FLAG(S_JSON_WEB_SPIFFS, data.web.use_spiffs);
        JSON_VALUE(S_JSON_LANG, data.web.lang);

        str.setCharAt(str.length()-1, '}');
    }

    // ========== PUBLIC ========== //
    void load() {
        debugF("Loading settings...");

        // read data from SPIFFS
        settings_t newData;

        // calc and check hash
        if (newData.magic_num == MAGIC_NUM) {
            data                  = newData;
            data.version.major    = DEAUTHER_VERSION_MAJOR;
            data.version.minor    = DEAUTHER_VERSION_MINOR;
            data.version.revision = DEAUTHER_VERSION_REVISION;
            debuglnF("OK");
            save();
        } else {
            debuglnF("Invalid Hash");

            /*debug(data.magic_num);
               debugF(" != ");
               debugln(MAGIC_NUM);*/

            reset();
            save();
        }

        // check and fix mac
        if (!macValid(data.wifi.mac_st)) getRandomMac(data.wifi.mac_st);
        if (!macValid(data.wifi.mac_ap)) getRandomMac(data.wifi.mac_ap);

        changed = true;
    }

    void reset() {
        data.magic_num = MAGIC_NUM;

        data.version.major    = DEAUTHER_VERSION_MAJOR;
        data.version.minor    = DEAUTHER_VERSION_MINOR;
        data.version.revision = DEAUTHER_VERSION_REVISION;

        data.attack.attack_all_ch         = ATTACK_ALL_CH;
        data.attack.random_tx             = RANDOM_TX;
        data.attack.timeout               = ATTACK_TIMEOUT;
        data.attack.deauths_per_target    = DEAUTHS_PER_TARGET;
        data.attack.deauth_reason         = DEAUTH_REASON;
        data.attack.beacon_interval       = beacon_interval_t::INTERVAL_100MS;
        data.attack.probe_frames_per_ssid = PROBE_FRAMES_PER_SSID;

        data.wifi.channel = 1;
        getRandomMac(data.wifi.mac_st);
        getRandomMac(data.wifi.mac_ap);

        data.sniffer.channel_time      = CH_TIME;
        data.sniffer.min_deauth_frames = MIN_DEAUTH_FRAMES;

        strncpy(data.ap.ssid, AP_SSID, 32);
        strncpy(data.ap.password, AP_PASSWD, 64);
        data.ap.hidden = AP_HIDDEN;
        uint8_t ip[4] = AP_IP_ADDR;

        memcpy(data.ap.ip, ip, 4);

        data.web.enabled        = WEB_ENABLED;
        data.web.captive_portal = WEB_CAPTIVE_PORTAL;
        data.web.use_spiffs     = WEB_USE_SPIFFS;
        memcpy(data.web.lang, DEFAULT_LANG, 3);
        

        changed = true;

        debuglnF("Settings reset to default");
    }

    void save(bool force) {
        if (force || changed) {
            // EEPROMHelper::saveObject(SETTINGS_ADDR, data);

            changed = false;

            String json_buffer;
            get_json(json_buffer);
            
            if (file_spiffs::writeFile(SETTINGS_PATH,json_buffer)) {
                debugF("Settings saved in ");
            } else {
                debugF("ERROR: saving ");
            }

            debugln(SETTINGS_PATH);
        }
    }

    void print() {
        String json_buffer;

        get_json(json_buffer);

        json_buffer.replace("\":", ": ");
        json_buffer.replace(": 0\r\n", ": false\r\n");
        json_buffer.replace(": 1\r\n", ": true\r\n");
        json_buffer.replace("\"", "");
        json_buffer.replace("{", "");
        json_buffer.replace("}", "");
        json_buffer.replace(",", "\r\n");

        debuglnF("[========== Settings ==========]");
        debugln(json_buffer);
    }
    
    const sniffer_settings_t& getSnifferSettings() {
        return data.sniffer;
    }
}