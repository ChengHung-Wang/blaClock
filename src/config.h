#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>

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
#define PIN_74HC595_OE 25
#define PIN_74HC595_STCP 0
#define PIN_74HC595_SHCP 33
// I2C
#define PIN_SCL 22
#define PIN_SDA 21


// ************************
// Internet Services Configuration
// ************************
#define DNS_PORT 53
#define WIFI_SSID "LouisaCoffee"
#define WIFI_PASSWORD "23676613"


//  temp
// ************************
// I2C scan
// ************************
void i2cScan()
{
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        // 0x32
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            Serial.println(String("address: ") + address);
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println("No I2C devices found\n");
    }
    else
    {
        Serial.println("done\n");
    }
}
#endif // CONFIG_H