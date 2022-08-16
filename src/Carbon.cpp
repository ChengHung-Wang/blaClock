#include <Carbon.h>

void Carbon::init() {
    configTime(0, 0, ntpServerURI.c_str());
    issetRTC = rtc.begin();
}

unsigned long Carbon::now() {
    time_t datetime;
    struct tm timeInfo;
    if (issetRTC && hasInitRTC) {
        return millis();
    }
    if (!getLocalTime(&timeInfo)) {
        // TODO: save to syslog
        //Serial.println("Failed to obtain time");
        return(0);
    }
    time(&datetime);
    updateRTC(timeInfo);
    hasInitRTC = true;
    return datetime;
}

void Carbon::updateRTC(struct tm timeInfo) {
    if (issetRTC) {
        rtc.adjust(DateTime(
        timeInfo.tm_year, 
        timeInfo.tm_mon, 
        timeInfo.tm_mday, 
        timeInfo.tm_hour, 
        timeInfo.tm_min,
        timeInfo.tm_sec
    ));
    }
}

void Carbon::api_now(AsyncWebServerRequest *req) {
    String jsonStr;
    DynamicJsonDocument result(4096);
    int responseCode;
    bool forceFromRTC = false;
    if (req->hasParam("rtc")) {
        forceFromRTC =  req->getParam("rtc")->value() == "true";
    }
    time_t dateNow = this->now();

    if (dateNow == 0 && !forceFromRTC) {
        responseCode = 500;
        result["success"] = false;
        result["message"] = "NTP_CLIENT_NOT_SET";
        result["data"] = "";
    }else {
        responseCode = 200;
        result["success"] = true;
        result["message"] = "";
        result["data"]["millis"] = dateNow;
        if (issetRTC) {
            DateTime rtcNow = rtc.now();
            result["data"]["rtc"]["year"] = rtcNow.year();
            result["data"]["rtc"]["month"] = rtcNow.month();
            result["data"]["rtc"]["day"] = rtcNow.day();
            result["data"]["rtc"]["hour"] = rtcNow.hour();
            result["data"]["rtc"]["min"] = rtcNow.minute();
            result["data"]["rtc"]["sec"] = rtcNow.second();
        }
        if (req->hasParam("format")) {
            String format = req->getParam("format")->value();
            char formattedTime[256] = { 0 };
            struct tm timeInfo;
            strftime(formattedTime, sizeof(formattedTime), format.c_str(), &timeInfo);
            result["data"]["format"] = formattedTime;
        }
    }
    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
}