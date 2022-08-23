#include "Dpot.h"
#include <Arduino.h>

void Dpot::begin()
{
    pinMode(_ss, OUTPUT);
    SPI.begin();
}

void Dpot::write(uint8_t value)
{
    Serial.println("change digitalVR value to: " + String(value));
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

void Dpot::api_write(AsyncWebServerRequest *req) 
{
    String jsonStr;
    DynamicJsonDocument result(4096);
    int responseCode;
    if (req->hasParam("vol"))
    {
        String vol = req->getParam("vol")->value();
        if (this->isValidNumber(vol)) {
            this->write(vol.toInt());
            responseCode = 200;
            result["success"] = true;
            result["message"] = "";
            result["data"] = "";
        }
        else 
        {
            responseCode = 400;
            result["success"] = false;
            result["message"] = "MSG_WRONG_DATA_TYPE";
            result["data"] = "";
        }
    }
    else
    {
        responseCode = 400;
        result["success"] = false;
        result["message"] = "ERR_MISSING_FIELD";
        result["data"] = "";
    }
    serializeJson(result, jsonStr);
    AsyncWebServerResponse *res = req->beginResponse(responseCode, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
}

bool Dpot::isValidNumber(String str){
    bool isNum = false;
    for(byte i=0; i < str.length(); i++)
    {
        isNum = isDigit(str.charAt(i)) || (str.charAt(i) == '+') || (str.charAt(i) == '.') || (str.charAt(i) == '-');
        if(!isNum) 
        {
            return false;
        }
    }
    return isNum;
}