// Dpot.h: MCP41010 library

#ifndef DPOT_H
#define DPOT_H

#include <Arduino.h>
#include <SPI.h>

#define DPOT_CMD_WRITE 0x11
#define DPOT_CMD_SHUTDOWN 0x21

class Dpot
{
public:
    Dpot(int8_t ss = SS) : _ss(ss) {}
    void begin();
    void write(uint8_t value);
    void open();

private:
    const SPISettings _SETTINGS;
    int8_t _ss;
    void _writeReg(uint8_t command, uint8_t value);
};

#endif // DPOT_H