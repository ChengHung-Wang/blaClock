#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay
{
public:
    Relay(int _pin_DS, int _pin_SHCP, int _pin_STCP, int _pin_OE) : DS(_pin_DS), SHCP(_pin_SHCP), STCP(_pin_STCP), OE(_pin_OE)
    {}
    void init();
    void on(int);
    void off(int);
    void test();
private:
    int DS, SHCP, STCP, OE;
    // SHCP: Clock pin, STCP: Latch pin, DS: Data pin
    bool data[8] = { false };
    void write();
    unsigned long lastTestTime = millis();
    byte getData(); // ConvertBoolArrayToByte
};

#endif // RELAY_H