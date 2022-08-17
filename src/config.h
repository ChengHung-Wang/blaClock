#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ************************
// Pins
// ************************
// SPI (VSPI) for SD and MCP41010 Digital POT
#define PIN_SD_SS SS // 5
// digital potentiometer
#define PIN_DPOT_A_SS 12
#define PIN_DPOT_B_SS 14
#define PIN_DPOT_C_SS 27
#define PIN_DPOT_D_SS 26
// PCM5102 I2S DAC
#define PIN_DAC_DOUT 16
#define PIN_DAC_LCK 17
#define PIN_DAC_BCK 4
// 74HC595
#define PIN_74HC595_DS 32
#define PIN_74HC595_STCP 25
#define PIN_74HC595_SHCP 33

// ************************
// Internet Services Configuration
// ************************
#define DNS_PORT 53
#define WIFI_SSID "research"
#define WIFI_PASSWORD "Skills39"

#endif // CONFIG_H