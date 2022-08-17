#include "Dpot.h"
#include <Arduino.h>

void Dpot::begin()
{
    pinMode(_ss, OUTPUT);
    SPI.begin();
}

void Dpot::write(uint8_t value)
{
    Dpot::_writeReg(DPOT_CMD_WRITE, value);
}

void Dpot::open()
{
    Dpot::_writeReg(DPOT_CMD_SHUTDOWN, 1);
}

void Dpot::_writeReg(uint8_t command, uint8_t value)
{
    SPI.beginTransaction(_SETTINGS);
    digitalWrite(_ss, LOW);

    SPI.write16(command);
    SPI.write(value);

    digitalWrite(_ss, HIGH);
    SPI.endTransaction();
}
