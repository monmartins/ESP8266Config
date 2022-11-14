/* This software is licensed under the MIT License: https://github.com/spacehuhntech/esp8266_deauther */

#include "settings.h"

#define MAGIC_NUM 3416245

extern void getRandomMac(uint8_t* mac); //functions.h

namespace settings {
    //
    settings_t data;
    //
    const attack_settings_t& getAttackSettings() {
        return data.attack;
    }

}