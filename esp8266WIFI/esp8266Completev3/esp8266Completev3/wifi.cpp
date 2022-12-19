#include "wifi.h"
// #include "functions.h"
#include "config.h"
#include "web.h"
#include "Scan.h"

extern Scan   scan; //INclude on .ino

namespace wifi {
    // ===== PRIVATE ===== //
    wifi_mode_t   mode;
    ap_settings_t ap_settings;

    ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
    
    IPAddress local_IP WEB_IP_ADDR;
    IPAddress gateway GATEWAY;
    IPAddress subnet SUBNET;
    
    void startNewAP(const char *ssid, const char *password);
    void startMultiClientAP(const char *ssid, const char *password);
    void runMultiClientAP();

    void listWifi();
    void update();

    void setSSID(String ssid) {
        if (ssid.length() > 32) {
            debuglnF("ERROR: SSID longer than 32 characters");
        } else {
            strncpy(ap_settings.ssid, ssid.c_str(), 32);
        }
    }

    void setPassword(String password) {
        if (password.length() > 64) {
            debuglnF("ERROR: Password longer than 64 characters");
        } else if (password.length() < 8) {
            debuglnF("ERROR: Password must be at least 8 characters long");
        } else {
            strncpy(ap_settings.password, password.c_str(), 64);
        }
    }

    void setChannel(uint8_t ch) {
        if ((ch < 1) || (ch > 14)) {
            debuglnF("ERROR: Channel must be withing the range of 1-14");
        } else {
            ap_settings.channel = ch;
        }
    }

    void setHidden(bool hidden) {
        ap_settings.hidden = hidden;
    }

    void setCaptivePortal(bool captivePortal) {
        ap_settings.captive_portal = captivePortal;
    }
    // ===== PRIVATE ===== //
    void startNewAP(const char *ssid, const char *password){
        uint8_t channel=10;
        bool    hidden=false;
        bool    captive_portal=false;
        setSSID(ssid);
        setPassword(password);
        setChannel(channel);
        setHidden(hidden);
        setCaptivePortal(captive_portal);

        //
        // web::dns.setErrorReplyCode(DNSReplyCode::NoError);
        web::startDNS(local_IP);
        //
        
        Serial.print("Version 1.2 -- ");
        Serial.print("Setting soft-AP configuration ... ");
        Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

        WiFi.softAP(ssid, password);             // Start the access point
        Serial.print("Access Point \"");
        Serial.print(ssid);
        Serial.println("\" started");

        Serial.print("IP address:\t");
        Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
    }

    void listWifi(){
        int numberOfNetworks = WiFi.scanNetworks();

        for(int i =0; i<numberOfNetworks; i++){
            Serial.print("Network name: ");
            Serial.println(WiFi.SSID(i));
            Serial.print("Signal strength: ");
            Serial.println(WiFi.RSSI(i));
            Serial.println("-----------------------");
        }
    }

    void startAP() {
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP(ap_settings.ssid, ap_settings.password, ap_settings.channel, ap_settings.hidden);

        // dns.setErrorReplyCode(DNSReplyCode::NoError);
        // dns.start(53, "*", ip);

        // MDNS.begin(WEB_URL);

        // server.on("/list", HTTP_GET, handleFileList); // list directory

        // #ifdef USE_PROGMEM_WEB_FILES
        // ================================================================
        // paste here the output of the webConverter.py
        // if (!config::getWebSettings().use_spiffs) {
        //     server.on("/", HTTP_GET, []() {
        //         sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
        //     });
        //     server.on("/index.html", HTTP_GET, []() {
        //         sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
        //     });
        //     server.on("/scan.html", HTTP_GET, []() {
        //         sendProgmem(scanhtml, sizeof(scanhtml), W_HTML);
        //     });
        //     server.on("/info.html", HTTP_GET, []() {
        //         sendProgmem(infohtml, sizeof(infohtml), W_HTML);
        //     });
        //     server.on("/ssids.html", HTTP_GET, []() {
        //         sendProgmem(ssidshtml, sizeof(ssidshtml), W_HTML);
        //     });
        //     server.on("/attack.html", HTTP_GET, []() {
        //         sendProgmem(attackhtml, sizeof(attackhtml), W_HTML);
        //     });
        //     server.on("/settings.html", HTTP_GET, []() {
        //         sendProgmem(settingshtml, sizeof(settingshtml), W_HTML);
        //     });
        //     server.on("/style.css", HTTP_GET, []() {
        //         sendProgmem(stylecss, sizeof(stylecss), W_CSS);
        //     });
        //     server.on("/js/ssids.js", HTTP_GET, []() {
        //         sendProgmem(ssidsjs, sizeof(ssidsjs), W_JS);
        //     });
        //     server.on("/js/site.js", HTTP_GET, []() {
        //         sendProgmem(sitejs, sizeof(sitejs), W_JS);
        //     });
        //     server.on("/js/attack.js", HTTP_GET, []() {
        //         sendProgmem(attackjs, sizeof(attackjs), W_JS);
        //     });
        //     server.on("/js/scan.js", HTTP_GET, []() {
        //         sendProgmem(scanjs, sizeof(scanjs), W_JS);
        //     });
        //     server.on("/js/settings.js", HTTP_GET, []() {
        //         sendProgmem(settingsjs, sizeof(settingsjs), W_JS);
        //     });
        //     server.on("/lang/hu.lang", HTTP_GET, []() {
        //         sendProgmem(hulang, sizeof(hulang), W_JSON);
        //     });
        //     server.on("/lang/ja.lang", HTTP_GET, []() {
        //         sendProgmem(jalang, sizeof(jalang), W_JSON);
        //     });
        //     server.on("/lang/nl.lang", HTTP_GET, []() {
        //         sendProgmem(nllang, sizeof(nllang), W_JSON);
        //     });
        //     server.on("/lang/fi.lang", HTTP_GET, []() {
        //         sendProgmem(filang, sizeof(filang), W_JSON);
        //     });
        //     server.on("/lang/cn.lang", HTTP_GET, []() {
        //         sendProgmem(cnlang, sizeof(cnlang), W_JSON);
        //     });
        //     server.on("/lang/ru.lang", HTTP_GET, []() {
        //         sendProgmem(rulang, sizeof(rulang), W_JSON);
        //     });
        //     server.on("/lang/pl.lang", HTTP_GET, []() {
        //         sendProgmem(pllang, sizeof(pllang), W_JSON);
        //     });
        //     server.on("/lang/uk.lang", HTTP_GET, []() {
        //         sendProgmem(uklang, sizeof(uklang), W_JSON);
        //     });
        //     server.on("/lang/de.lang", HTTP_GET, []() {
        //         sendProgmem(delang, sizeof(delang), W_JSON);
        //     });
        //     server.on("/lang/it.lang", HTTP_GET, []() {
        //         sendProgmem(itlang, sizeof(itlang), W_JSON);
        //     });
        //     server.on("/lang/en.lang", HTTP_GET, []() {
        //         sendProgmem(enlang, sizeof(enlang), W_JSON);
        //     });
        //     server.on("/lang/fr.lang", HTTP_GET, []() {
        //         sendProgmem(frlang, sizeof(frlang), W_JSON);
        //     });
        //     server.on("/lang/in.lang", HTTP_GET, []() {
        //         sendProgmem(inlang, sizeof(inlang), W_JSON);
        //     });
        //     server.on("/lang/ko.lang", HTTP_GET, []() {
        //         sendProgmem(kolang, sizeof(kolang), W_JSON);
        //     });
        //     server.on("/lang/ro.lang", HTTP_GET, []() {
        //         sendProgmem(rolang, sizeof(rolang), W_JSON);
        //     });
        //     server.on("/lang/da.lang", HTTP_GET, []() {
        //         sendProgmem(dalang, sizeof(dalang), W_JSON);
        //     });
        //     server.on("/lang/ptbr.lang", HTTP_GET, []() {
        //         sendProgmem(ptbrlang, sizeof(ptbrlang), W_JSON);
        //     });
        //     server.on("/lang/cs.lang", HTTP_GET, []() {
        //         sendProgmem(cslang, sizeof(cslang), W_JSON);
        //     });
        //     server.on("/lang/tlh.lang", HTTP_GET, []() {
        //         sendProgmem(tlhlang, sizeof(tlhlang), W_JSON);
        //     });
        //     server.on("/lang/es.lang", HTTP_GET, []() {
        //         sendProgmem(eslang, sizeof(eslang), W_JSON);
        //     });
        //     server.on("/lang/th.lang", HTTP_GET, []() {
        //         sendProgmem(thlang, sizeof(thlang), W_JSON);
        //     });
        // }
        // server.on("/lang/default.lang", HTTP_GET, []() {
        //     if (!settings::getWebSettings().use_spiffs) {
        //         if (String(settings::getWebSettings().lang) == "hu") sendProgmem(hulang, sizeof(hulang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "ja") sendProgmem(jalang, sizeof(jalang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "nl") sendProgmem(nllang, sizeof(nllang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "fi") sendProgmem(filang, sizeof(filang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "cn") sendProgmem(cnlang, sizeof(cnlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "ru") sendProgmem(rulang, sizeof(rulang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "pl") sendProgmem(pllang, sizeof(pllang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "uk") sendProgmem(uklang, sizeof(uklang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "de") sendProgmem(delang, sizeof(delang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "it") sendProgmem(itlang, sizeof(itlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "en") sendProgmem(enlang, sizeof(enlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "fr") sendProgmem(frlang, sizeof(frlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "in") sendProgmem(inlang, sizeof(inlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "ko") sendProgmem(kolang, sizeof(kolang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "ro") sendProgmem(rolang, sizeof(rolang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "da") sendProgmem(dalang, sizeof(dalang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "ptbr") sendProgmem(ptbrlang, sizeof(ptbrlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "cs") sendProgmem(cslang, sizeof(cslang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "tlh") sendProgmem(tlhlang, sizeof(tlhlang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "es") sendProgmem(eslang, sizeof(eslang), W_JSON);
        //         else if (String(settings::getWebSettings().lang) == "th") sendProgmem(thlang, sizeof(thlang), W_JSON);

        //         else handleFileRead("/web/lang/"+String(settings::getWebSettings().lang)+".lang");
        //     } else {
        //         handleFileRead("/web/lang/"+String(settings::getWebSettings().lang)+".lang");
        //     }
        // });
        // // ================================================================
        // #endif /* ifdef USE_PROGMEM_WEB_FILES */

        // server.on("/run", HTTP_GET, []() {
        //     server.send(200, str(W_TXT), str(W_OK).c_str());
        //     String input = server.arg("cmd");
        //     cli.exec(input);
        // });

        // server.on("/attack.json", HTTP_GET, []() {
        //     server.send(200, str(W_JSON), attack.getStatusJSON());
        // });

        // // called when the url is not defined here
        // // use it to load content from SPIFFS
        // server.onNotFound([]() {
        //     if (!handleFileRead(server.uri())) {
        //         if (settings::getWebSettings().captive_portal) sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
        //         else server.send(404, str(W_TXT), str(W_FILE_NOT_FOUND));
        //     }
        // });

        // server.begin();
        // mode = wifi_mode_t::ap;

        // prntln(W_STARTED_AP);
        // printStatus();
    }
    void stopAP() {
        if (mode == wifi_mode_t::ap) {
            wifi_promiscuous_enable(0);
            WiFi.persistent(false);
            WiFi.disconnect(true);
            wifi_set_opmode(STATION_MODE);
            // prntln(W_STOPPED_AP);
            mode = wifi_mode_t::st;
        }
    }

    void resumeAP() {
        if (mode != wifi_mode_t::ap) {
            mode = wifi_mode_t::ap;
            wifi_promiscuous_enable(0);
            WiFi.softAPConfig(local_IP, local_IP, subnet);
            WiFi.softAP(ap_settings.ssid, ap_settings.password, ap_settings.channel, ap_settings.hidden);
            // prntln(W_STARTED_AP);
        }
    }

    void update() {
        if ((mode != wifi_mode_t::off) && !scan.isScanning()) {
            web::loopServer();
            web::loopDNS();
            // web::dns.processNextRequest();
        }
    }


    void startMultiClientAP(const char *ssid, const char *password){
        wifiMulti.addAP(ssid, password);
    }
    void runMultiClientAP(){
        while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
            delay(250);
            Serial.print('.');
        }
    }
}