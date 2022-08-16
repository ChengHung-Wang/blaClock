#ifndef GAUGE_H
#define GAUGE_H

#include <Arduino.h>

class Gauge
{
public:
    Gauge(uint8_t addr = 0x32) : addr_(addr){};
    bool begin();
    uint16_t voltage(); // unit: mV; interval: 500ms
    uint8_t charge();   // unit: percent
                        // TODO make a decimal version of charge()

private:
    bool _ping();
    uint16_t readReg(uint8_t);

    const uint8_t REG_VCELL = 0x2;
    const uint8_t REG_SOC = 0x4;

    uint8_t addr_;
};

#endif // GAUGE_H