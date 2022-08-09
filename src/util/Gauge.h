#ifndef GAUGE_H
#define GAUGE_H

class Gauge
{
public:
  Gauge(int scl, int sda, byte addr = 0x32) : scl_(scl), sda_(sda), addr_(addr){};
  bool begin();
  uint16_t voltage(); // unit: mV; interval: 500ms
  uint8_t charge();   // unit: percent
  // TODO make a decimal version of charge()

private:
  bool ping();
  uint16_t readReg(uint8_t);

  const uint8_t REG_VCELL = 0x2;
  const uint8_t REG_SOC = 0x4;

  TwoWire wire_{1};
  int scl_;
  int sda_;
  uint8_t addr_;
};

#endif // GAUGE_H