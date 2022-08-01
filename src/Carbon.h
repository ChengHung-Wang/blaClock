#include <Arduino.h>
#include <time.h>

class Carbon
{
public:
    Carbon()
    {}
    Carbon(String _url) : ntpServerURI(_url)
    {}
    void init();
    unsigned long now();
private:
    String ntpServerURI = "pool.ntp.org";
};