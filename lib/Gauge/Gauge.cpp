#include "Gauge.h"
#include <Arduino.h>
#include <Wire.h>

bool Gauge::begin()
{
    Wire.begin();
    return this->_ping();
}

bool Gauge::begin(int scl, int sda)
{
    Wire.begin(scl, sda);
    return this->_ping();
}

bool Gauge::_ping()
{
    Wire.beginTransmission(addr_);
    byte err = Wire.endTransmission();
    return err == 0;
}

uint16_t Gauge::voltage()
{
    uint16_t data = readReg(REG_VCELL);
    return (data >> 4) / 1.25;
}

uint8_t Gauge::charge()
{
    uint16_t data = readReg(REG_SOC);
    return data >> 8;
}

uint16_t Gauge::readReg(uint8_t reg)
{
    Wire.beginTransmission(addr_);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(addr_, (uint8_t)2);
    uint8_t high_byte = Wire.read();
    uint8_t low_byte = Wire.read();
    return high_byte << 8 | low_byte;
}