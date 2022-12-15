#pragma once

extern "C" {
  #include "user_interface.h" ///Users/ramonmartins/Library/Arduino15/packages/esp8266/hardware/esp8266/3.0.2/tools/sdk/include/user_interface.h
}
#include "Arduino.h"
#include <String>

/*
   Here is a collection of useful functions and variables.
   They are used globally via an 'extern' reference in every class.
   Making everything static will lead to problems with the Arduino ESP8266 2.0.0 SDK,
   there were some fixed in later version but we need to use the old version for injecting deauth packets.
 */

uint8_t broadcast[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t wifi_channel = 1;

/* ===== VENDOR LIST (oui.h) ===== */
void getRandomMac(uint8_t* mac) {
    int num = random(sizeof(data_vendors) / 11 - 1);
    uint8_t i;

    for (i = 0; i < 3; i++) mac[i] = pgm_read_byte_near(data_macs + num * 5 + i); //oui.h data_macs from .ino

    for (i = 3; i < 6; i++) mac[i] = random(256);
}
//
// equals function
bool eqls(const char* str, const char* keywordPtr) {
    if (strlen(str) > 255) return false;  // when string too long

    char keyword[strlen_P(keywordPtr) + 1];

    strcpy_P(keyword, keywordPtr);

    uint8_t lenStr     = strlen(str);
    uint8_t lenKeyword = strlen(keyword);

    if (lenStr > lenKeyword) return false;  // string can't be longer than keyword (but can be smaller because of '/'

    // and ',')

    uint8_t a      = 0;
    uint8_t b      = 0;
    bool    result = true;

    while (a < lenStr && b < lenKeyword) {
        if ((keyword[b] == SLASH) || (keyword[b] == COMMA)) b++;

        if (tolower(str[a]) != tolower(keyword[b])) result = false;

        if (((a == lenStr) && !result) || !result) { // fast forward to next comma
            while (b < lenKeyword && keyword[b] != COMMA) b++;
            result = true;
            a      = 0;
        } else {
            a++;
            b++;
        }
    }
    // comparison correct AND string checked until the end AND keyword checked until the end
    return result && a == lenStr && (keyword[b] == COMMA || keyword[b] == SLASH || keyword[b] == ENDOFLINE);
}

bool eqls(String str, const char* keywordPtr) {
    return eqls(str.c_str(), keywordPtr);
}

//
//
// boolean to string
String b2s(bool input) {
    return String(input ? STR_TRUE : STR_FALSE); //STR_TRUE and STR_FALSE functions.h
}

// boolean to asterix *
String b2a(bool input) {
    return input ? String(ASTERIX) : String(SPACE);
}

// string to boolean
bool s2b(String input) {
    return eqls(input, STR_TRUE);
}
//
// ===== PRINT FUNCTIONS ===== //
void prnt(const String s) {
    Serial.print(s);
}

void prnt(const bool b) {
    Serial.print(b2s(b));
}

void prnt(const char c) {
    Serial.print(c);
}

void prnt(const char* ptr) {
    Serial.print(FPSTR(ptr));
}

void prnt(const char* ptr, int len) {
    for (int i = 0; i<len; i++) prnt(ptr[i]);
}

void prnt(const int i) {
    Serial.print((String)i);
}

void prnt(const uint32_t i) {
    Serial.printf("%u", i);
}
//

//
void prntln() {
    Serial.println();
}

void prntln(const String s) {
    Serial.println(s);
}

void prntln(const bool b) {
    Serial.println(b2s(b));
}

void prntln(const char c) {
    Serial.println(c);
}

void prntln(const char* ptr) {
    Serial.println(FPSTR(ptr));
}

void prntln(const char* ptr, int len) {
    for (int i = 0; i<len; i++) prnt(ptr[i]);
    prntln();
}

void prntln(const int i) {
    Serial.println((String)i);
}

void prntln(const uint32_t i) {
    Serial.printf("%u\r\n", i);
}
//
//
/* ===== MAC ADDRESSES ===== */
bool macBroadcast(uint8_t* mac) {
    for (uint8_t i = 0; i < 6; i++)
        if (mac[i] != broadcast[i]) return false;

    return true;
}

bool macValid(uint8_t* mac) {
    for (uint8_t i = 0; i < 6; i++)
        if (mac[i] != 0x00) return true;

    return false;
}

bool macMulticast(uint8_t* mac) {
    // see https://en.wikipedia.org/wiki/Multicast_address
    if ((mac[0] == 0x33) && (mac[1] == 0x33)) return true;

    if ((mac[0] == 0x01) && (mac[1] == 0x80) && (mac[2] == 0xC2)) return true;

    if ((mac[0] == 0x01) && (mac[1] == 0x00) && ((mac[2] == 0x5E) || (mac[2] == 0x0C))) return true;

    if ((mac[0] == 0x01) && (mac[1] == 0x0C) && (mac[2] == 0xCD) &&
        ((mac[3] == 0x01) || (mac[3] == 0x02) || (mac[3] == 0x04)) &&
        ((mac[4] == 0x00) || (mac[4] == 0x01))) return true;

    if ((mac[0] == 0x01) && (mac[1] == 0x00) && (mac[2] == 0x0C) && (mac[3] == 0xCC) && (mac[4] == 0xCC) &&
        ((mac[5] == 0xCC) || (mac[5] == 0xCD))) return true;

    if ((mac[0] == 0x01) && (mac[1] == 0x1B) && (mac[2] == 0x19) && (mac[3] == 0x00) && (mac[4] == 0x00) &&
        (mac[5] == 0x00)) return true;

    return false;
}

