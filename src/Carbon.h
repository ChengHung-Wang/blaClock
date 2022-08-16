#include <Arduino.h>
#include <Time.h>
// RTC support
#include <RTClib.h>
// JSON support
#include <ArduinoJson.h>
// Network, Web Service
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class Carbon
{
public:
    Carbon()
    {}
    Carbon(String _url) : ntpServerURI(_url)
    {}
    void init();
    unsigned long now();
    void api_now(AsyncWebServerRequest*);
    void updateRTC(struct tm);
    
private:
    String ntpServerURI = "pool.ntp.org";
    bool hasInitRTC = false;
    bool issetRTC = false;
    RTC_DS3231 rtc;
};