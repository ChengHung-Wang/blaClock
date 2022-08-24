#include "Relay.h"

void Relay::init()
{
    pinMode(DS, OUTPUT);
    pinMode(SHCP, OUTPUT);  
    pinMode(STCP, OUTPUT);
    pinMode(OE, OUTPUT);
    digitalWrite(OE, HIGH); // disable ouput
    digitalWrite(STCP, LOW);
    shiftOut(DS, SHCP, LSBFIRST, B00000000);
    digitalWrite(STCP, HIGH);
    digitalWrite(OE, LOW); // enable output
}

void Relay::on(int number)
{
    if (number > (sizeof(this->data) / sizeof(int)) || number <= 0) 
    {
        return;
    }
    data[number - 1] = true;
    this->write();
}

void Relay::off(int number)
{
    if (number > (sizeof(this->data) / sizeof(int)) || number <= 0) 
    {
        return;
    }
    data[number - 1] = false;
    this->write();
}

void Relay::write() {
    digitalWrite(STCP, LOW);
    shiftOut(DS, SHCP, LSBFIRST, this->getData());
    digitalWrite(STCP, HIGH);
}

void Relay::test() {
    int interval = 1000; // 300ms
    if (millis() - this->lastTestTime < interval)
    {
        return;
    }

    for (int index = 0; index < sizeof(this->data); index ++) 
    {
        for (int index2 = 0; index2 < sizeof(this->data); index2 ++) 
        {
            if (index2 == index) 
            {
                this->on(index + 1);
                Serial.println("test 74HCXXX pin" + String(index + 1) + " open");
            }
            else 
            {
                this->off(index + 1);
            }
        }
    }
    lastTestTime = millis();
}

byte Relay::getData()
{
    byte result = 0;
    // This assumes the array never contains more than 8 elements!
    int index = 8 - sizeof(this->data);

    // Loop through the array
    for (const bool &item : this->data) 
    {
        if (item) 
        {
            result |= (byte)(1 << (7 - index));
        }
    }
    return result;
}