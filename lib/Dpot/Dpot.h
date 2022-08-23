// Dpot.h: MCP41010 library

#ifndef DPOT_H
#define DPOT_H

#include <Arduino.h>
#include <String.h>
#include <SPI.h>
// JSON support
#include <ArduinoJson.h>
// Network, Web Service
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define DPOT_CMD_WRITE 0x11
#define DPOT_CMD_SHUTDOWN 0x21

class Dpot
{
public:
    Dpot(int8_t ss = SS) : _ss(ss) {}
    void begin();
    void write(uint8_t value);
    void open();
    void api_write(AsyncWebServerRequest*);

private:
    const SPISettings _SETTINGS;
    int8_t _ss;
    void _writeReg(uint8_t command, uint8_t value);
    bool isValidNumber(String);
};

#endif // DPOT_H