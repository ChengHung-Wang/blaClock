#include "Relay.h"

void Relay::init()
{
    pinMode(pin, OUTPUT);
}

void Relay::on()
{
    digitalWrite(pin, HIGH);
}

void Relay::off()
{
    digitalWrite(pin, LOW);
}