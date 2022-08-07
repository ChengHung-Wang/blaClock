#include <Carbon.h>

void Carbon::init() {
    configTime(0, 0, ntpServerURI.c_str());
}

unsigned long Carbon::now() {
    time_t now;
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo)) {
        // TODO: save to syslog
        //Serial.println("Failed to obtain time");
        return(0);
    }
    time(&now);
    return now;
}