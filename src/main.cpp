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

#include <config.h>

unsigned long previousMillis = 0;
unsigned long interval = 30000;

// ************************
// web server test
// ************************
Carbon DateTime;
FileServer SDCard(5);
AsyncWebServer Server(80);
DNSServer DNS;
Audio audio;

// Create a MAX17043
Gauge gauge;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

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

void i2cScan()
{
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        // 0x32
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            Serial.println(String("address: ") + address);
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println("No I2C devices found\n");
    }
    else
    {
        Serial.println("done\n");
    }
}

void setup()
{
    Serial.begin(115200);
    // Wire.begin(PIN_SCL, PIN_SDA);
    //  i2cScan();
    //  init
    IPAddress ip = initWiFi();
    DNS.start(DNS_PORT, "blaclock.net", ip);
    SDCard.init();
    DateTime.init();

    if (!gauge.begin())
    {
        Serial.println("NMSL");
    }

    // ----------------------
    // Web Service
    // ----------------------
    Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    File responseContent = SDCard.open("/dist/index.html");
    AsyncWebServerResponse *res = request->beginResponse(responseContent, "/", "text/html", false);
    res->addHeader("charset", "UTF-8");
    res->setCode(200);
    request->send(res); });
    Server.onNotFound([](AsyncWebServerRequest *req)
                      { SDCard.api_webuiDependsFile(req); });

    // ----------------------
    // restful API
    // ----------------------
    Server.on("/api/v1/dir", HTTP_GET, [](AsyncWebServerRequest *req)
              { SDCard.api_listDir(req); });
    Server.on("/api/v1/dir", HTTP_POST, [](AsyncWebServerRequest *req)
              { SDCard.api_createDir(req); });
    Server.on("/api/v1/dir", HTTP_DELETE, [](AsyncWebServerRequest *req)
              { SDCard.api_deleteDir(req); });
    Server.on("/api/v1/file", HTTP_GET, [](AsyncWebServerRequest *req)
              { SDCard.api_fileOpen(req); });
    Server.on("/api/v1/file", HTTP_POST, [](AsyncWebServerRequest *req) { /* TODO: file upload */ });
    Server.on("/api/v1/file", HTTP_PUT, [](AsyncWebServerRequest *req)
              { SDCard.api_renameFile(req); });
    Server.on("/api/v1/file", HTTP_DELETE, [](AsyncWebServerRequest *req)
              { SDCard.api_deleteFile(req); });

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
    Server.on("/api/v1/audio/volume", HTTP_GET, [](AsyncWebServerRequest *req) {});  // get all channel volumes
    Server.on("/api/v1/audio/volume", HTTP_POST, [](AsyncWebServerRequest *req) {}); // set someone channel's vol

    Server.begin();
}

void loop()
{
    Serial.println("voltage: " + String(gauge.voltage()));
    Serial.println("charge: " + String(gauge.charge()));
    delay(3000);
    // // put your main code here, to run repeatedly:
    // //server.handleClient();
    // unsigned long currentMillis = millis();
    // if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    //   Serial.print(millis());
    //   Serial.println("Reconnecting to WiFi...");
    //   WiFi.disconnect();
    //   WiFi.reconnect();
    //   previousMillis = currentMillis;
    // }
    // audio.loop();
}