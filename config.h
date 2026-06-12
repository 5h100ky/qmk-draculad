#pragma once

// Split
#define EE_HANDS
#define SPLIT_POINTING_ENABLE

// Pimoroni Trackball 
#define POINTING_DEVICE_RIGHT             
#define PIMORONI_TRACKBALL_INVERT_X       


// I2C 
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

// OLED
#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X64
#    define OLED_TIMEOUT 30000
#endif