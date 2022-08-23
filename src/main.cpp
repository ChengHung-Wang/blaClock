#include <Arduino.h>
// Network, Web Service
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
// JSON support
#include <ArduinoJson.h>

// I2S Audio
#include <Audio.h>
// Time
#include <Carbon.h>
// MAX17043
#include <Gauge.h>
// SD Card
#include <FileServer.h>
// Relay Control
#include <Relay.h>
// MCP4XXXX Chip
// #include <MCP4XXXX.h>        
#include "../lib/Dpot/Dpot.h"

#include <config.h>

// ************************
// web server test
// ************************
Carbon DateTime;
FileServer SDCard(5);
AsyncWebServer Server(80);
DNSServer DNS;
Audio PCM5102A;
// MCP41010
Dpot digitalVR1(PIN_DPOT_A_SS);
Dpot digitalVR2(PIN_DPOT_B_SS);
Dpot digitalVR3(PIN_DPOT_C_SS);
Dpot digitalVR4(PIN_DPOT_D_SS);

// Create a MAX17043
Gauge gauge;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30;

// Variable to save current epoch time
unsigned long epochTime;

// Initialize WiFi
IPAddress initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(String(WIFI_SSID).c_str(), String(WIFI_PASSWORD).c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return WiFi.localIP();
}

void setup()
{
    Serial.begin(115200);
    //  init
    IPAddress ip = initWiFi();
    DNS.start(DNS_PORT, "blaclock.net", ip);
    SDCard.init();
    DateTime.init();

    digitalVR1.begin();
    digitalVR2.begin();
    digitalVR3.begin();
    digitalVR4.begin();

    digitalVR1.write(0x00);
    digitalVR2.write(0x00);
    digitalVR3.write(0x00);
    digitalVR4.write(0x00);

    PCM5102A.setPinout(PIN_DAC_BCK, PIN_DAC_LCK, PIN_DAC_DOUT);
    PCM5102A.setVolume(12); // 0...21
    // PCM5102A.connecttoFS(SDCard.card, "/ProDer.mp3");

    if (!gauge.begin(PIN_SCL, PIN_SDA))
    {
        Serial.println("NMSL");
    }

    Serial.println("now: " + String(DateTime.now()));

    // ----------------------
    // Web Service
    // ----------------------
    Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        File responseContent = SDCard.open("/dist/index.html");
        AsyncWebServerResponse *res = request->beginResponse(responseContent, "/", "text/html", false);
        res->addHeader("charset", "UTF-8");
        res->setCode(200);
        request->send(res); 
    });

    Server.onNotFound([](AsyncWebServerRequest *req){ SDCard.api_webuiDependsFile(req); });

    // ----------------------
    // restful API
    // ----------------------
    Server.on("/api/v1/dir", HTTP_GET, [](AsyncWebServerRequest *req){ SDCard.api_listDir(req); });
    Server.on("/api/v1/dir", HTTP_POST, [](AsyncWebServerRequest *req){ SDCard.api_createDir(req); });
    Server.on("/api/v1/dir", HTTP_DELETE, [](AsyncWebServerRequest *req){ SDCard.api_deleteDir(req); });
    Server.on("/api/v1/file", HTTP_GET, [](AsyncWebServerRequest *req){ SDCard.api_fileOpen(req); });
    Server.on("/api/v1/file", HTTP_POST, [](AsyncWebServerRequest *req) { /* TODO: file upload */ });
    Server.on("/api/v1/file", HTTP_PUT, [](AsyncWebServerRequest *req){ SDCard.api_renameFile(req); });
    Server.on("/api/v1/file", HTTP_DELETE, [](AsyncWebServerRequest *req){ SDCard.api_deleteFile(req); });

    Server.on("/api/v1/clock", HTTP_GET, [](AsyncWebServerRequest *req) {});    // get clock setting
    Server.on("/api/v1/clock", HTTP_POST, [](AsyncWebServerRequest *req) {});   // create a clock time
    Server.on("/api/v1/clock", HTTP_PUT, [](AsyncWebServerRequest *req) {});    // update clock
    Server.on("/api/v1/clock", HTTP_DELETE, [](AsyncWebServerRequest *req) {}); // delete clock

    Server.on("api/v1/clock/alert", HTTP_GET, [](AsyncWebServerRequest *req) {});        // alert summary
    Server.on("api/v1/clock/alert", HTTP_POST, [](AsyncWebServerRequest *req) {});       // alert set
    Server.on("api/v1/clock/alert/unlock", HTTP_ANY, [](AsyncWebServerRequest *req) {}); // unlock alert

    Server.on("/api/v1/time", HTTP_GET, [](AsyncWebServerRequest *req) {});  // get current time
    Server.on("/api/v1/time", HTTP_POST, [](AsyncWebServerRequest *req) {}); // modify current time

    Server.on("/api/v1/notification", HTTP_GET, [](AsyncWebServerRequest *req) {});
    Server.on("/api/v1/notification", HTTP_POST, [](AsyncWebServerRequest *req) {});
    Server.on("/api/v1/notification", HTTP_PUT, [](AsyncWebServerRequest *req) {});
    Server.on("/api/v1/notification", HTTP_DELETE, [](AsyncWebServerRequest *req) {});

    Server.on("/api/v1/audio", HTTP_GET, [](AsyncWebServerRequest *req) {});         // get audio summary
    Server.on("/api/v1/audio/mode", HTTP_POST, [](AsyncWebServerRequest *req) {});   // mode = bluetooth, sd card, idle
    Server.on("/api/v1/audio/volume", HTTP_GET, [](AsyncWebServerRequest *req) {  });
    Server.on("/api/v1/audio/volume/1", HTTP_POST, [](AsyncWebServerRequest *req) { digitalVR1.api_write(req); });
    Server.on("/api/v1/audio/volume/2", HTTP_POST, [](AsyncWebServerRequest *req) { digitalVR2.api_write(req); });
    Server.on("/api/v1/audio/volume/3", HTTP_POST, [](AsyncWebServerRequest *req) { digitalVR3.api_write(req); });
    Server.on("/api/v1/audio/volume/4", HTTP_POST, [](AsyncWebServerRequest *req) { digitalVR4.api_write(req); });
    Server.begin();
}

void loop()
{
    if (DateTime.now() - lastTime >= timerDelay) {
        lastTime = DateTime.now();
        Serial.println("voltage: " + String(gauge.voltage()));
        Serial.println("charge: " + String(gauge.charge()));
        Serial.println("now: " + String(DateTime.now()));        
    }

    PCM5102A.loop();
}