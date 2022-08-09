#include <Arduino.h>
#include <Wire.h>
#include "Gauge.h"

Gauge::Gauge(int _scl, int _sda, byte _addr = 0x32)
{
  scl = _scl;
  sda = _sda;
  addr = _addr;
}

bool Gauge::begin()
{
  Wire.begin(addr, sda, scl);
  return this->ping();
}

bool Gauge::ping()
{
  Wire.beginTransmission(addr);
  byte err = Wire.endTransmission();
  if (err)
    return false;
  return true;
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
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, (uint8_t)2);
  uint8_t high_byte = Wire.read();
  uint8_t low_byte = Wire.read();
  return high_byte << 8 | low_byte;
}