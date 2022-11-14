#pragma once

extern "C" {
  #include "user_interface.h" //pgm_read_byte_near
}

/* ===== VENDOR LIST (oui.h) ===== */
void getRandomMac(uint8_t* mac) {
    int num = random(sizeof(data_vendors) / 11 - 1);
    uint8_t i;

    for (i = 0; i < 3; i++) mac[i] = pgm_read_byte_near(data_macs + num * 5 + i); //oui.h data_macs from .ino

    for (i = 3; i < 6; i++) mac[i] = random(256);
}