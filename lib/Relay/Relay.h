#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay
{
public:
    Relay(int _pin) : pin(_pin)
    {}
    void init();
    void on();
    void off();
private:
    int pin;
};

#endif // RELAY_H