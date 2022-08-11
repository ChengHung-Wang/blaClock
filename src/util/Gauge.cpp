#include <Arduino.h>
#include <Wire.h>
#include "Gauge.h"

bool Gauge::begin()
{
  wire_.begin(sda_, scl_);
  return this->ping();
}

bool Gauge::ping()
{
  wire_.beginTransmission(addr_);
  byte err = wire_.endTransmission();
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
  wire_.beginTransmission(addr_);
  wire_.write(reg);
  wire_.endTransmission(false);
  wire_.requestFrom(addr_, (uint8_t)2);
  uint8_t high_byte = wire_.read();
  uint8_t low_byte = wire_.read();
  return high_byte << 8 | low_byte;
}