#pragma once

// Split — 항상 좌측을 USB(마스터)에 연결
#define MASTER_LEFT
#define SPLIT_POINTING_ENABLE

// Pimoroni Trackball
#define POINTING_DEVICE_RIGHT


// I2C
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#define I2C1_CLOCK_SPEED 400000

// Pimoroni Trackball I2C address (default: 0x0A)
#define PIMORONI_TRACKBALL_ADDRESS 0x0A

// OLED
#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X64
#    define OLED_TIMEOUT 30000
#endif