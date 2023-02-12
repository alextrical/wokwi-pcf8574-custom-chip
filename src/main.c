// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha
//
// SPDX-License-Identifier: MIT
// Copyright (C) 2022 Uri Shaked / wokwi.com

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

const int ADDRESS = 0x38;

typedef struct {
    pin_t PIN_SDA;
    pin_t PIN_SCL;
    pin_t PIN_INT;
    pin_t PIN_A0;
    pin_t PIN_A1;
    pin_t PIN_A2;
    pin_t PIN_P0;
    pin_t PIN_P1;
    pin_t PIN_P2;
    pin_t PIN_P3;
    pin_t PIN_P4;
    pin_t PIN_P5;
    pin_t PIN_P6;
    pin_t PIN_P7;
    uint8_t counter;
} chip_state_t;

static bool on_i2c_connect(void *user_data, uint32_t address, bool connect);
static uint8_t on_i2c_read(void *user_data);
static bool on_i2c_write(void *user_data, uint8_t data);
static void on_i2c_disconnect(void *user_data);

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->PIN_INT = pin_init("INT", INPUT);
  chip->PIN_A0 = pin_init("A0", INPUT);
  chip->PIN_A1 = pin_init("A1", INPUT);
  chip->PIN_A2 = pin_init("A2", INPUT);
  chip->PIN_P0 = pin_init("P0", INPUT_PULLUP);
  chip->PIN_P1 = pin_init("P1", INPUT_PULLUP);
  chip->PIN_P2 = pin_init("P2", INPUT_PULLUP);
  chip->PIN_P3 = pin_init("P3", OUTPUT);
  chip->PIN_P4 = pin_init("P4", OUTPUT);
  chip->PIN_P5 = pin_init("P5", OUTPUT);
  chip->PIN_P6 = pin_init("P6", OUTPUT);
  chip->PIN_P7 = pin_init("P7", OUTPUT);
  chip->counter = 0;

  const i2c_config_t i2c_config = {
    .user_data = chip,
    .address = ADDRESS,
    .scl = pin_init("SCL", INPUT),
    .sda = pin_init("SDA", INPUT),
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = on_i2c_disconnect, // Optional
  };
  i2c_init(&i2c_config);

  
  // The following message will appear in the browser's DevTools console:
  // printf("Hello from custom chip!\n");
  printf("Hello from 24c01 at address %x \n", ADDRESS);
}


static void update_pins(chip_state_t *chip)
{
    for (uint8_t i = 0; i < 8; i++) {
        
        switch (i)
        {
            case 0:
                pin_write(chip->PIN_P0, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 1:
                pin_write(chip->PIN_P1, chip->counter & (1 << i) ? HIGH : LOW);
            break;
            
            case 2:
                pin_write(chip->PIN_P2, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 3:
                pin_write(chip->PIN_P3, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 4:
                pin_write(chip->PIN_P4, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 5:
                pin_write(chip->PIN_P5, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 6:
                pin_write(chip->PIN_P6, chip->counter & (1 << i) ? HIGH : LOW);
            break;

            case 7:
                pin_write(chip->PIN_P7, chip->counter & (1 << i) ? HIGH : LOW);
            break;

        }
    }
}

bool on_i2c_connect(void *user_data, uint32_t address, bool connect) {
  return true; /* Ack */
}

uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = user_data;
  chip->counter = 0;
  chip->counter |= pin_read(chip->PIN_P7);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P6);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P5);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P4);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P3);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P2);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P1);
  chip->counter<<=1;
  chip->counter |= pin_read(chip->PIN_P0);
  
  return chip->counter;
}

bool on_i2c_write(void *user_data, uint8_t data) {
  chip_state_t *chip = user_data;
  chip->counter = data;
  update_pins(chip);
  return true; // Ack
}


void on_i2c_disconnect(void *user_data) {
  // Do nothing
}
