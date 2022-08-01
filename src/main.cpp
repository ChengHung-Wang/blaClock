#include <Arduino.h>
#include <WiFi.h>
#include <Carbon.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// TODO: SDCard 與 MCP41010共用SPI, modify: util/MCP4XXXX
/*
  IO expender:
    input: 74hc165(三個EC11開關、主開關)
    output: 74hc595（五個Relay Control，3pin for RGB LED）
*/
#include <FileSystem.h>
#include <ArxContainer.h>
#include <util/Relay.h>

unsigned long previousMillis = 0;
unsigned long interval = 30000;

// ************************
// Pins
// ************************
// SPI (VSPI) for SD and MCP41010 Digital POT
#define PIN_SCK 18
#define PIN_MOSI 23
#define PIN_MISO 19
#define PIN_SD_SS 5
#define PIN_POTA_SS 32
#define PIN_POTB_SS 33
#define PIN_POTC_SS 25
#define PIN_POTD_SS 26
// I2C for MAX17043
#define PIN_SCL 22
#define PIN_SDA 21
// PCM5102 I2S DAC
#define PIN_DACA_DOUT 27
#define PIN_DACA_LRC 14
#define PIN_DACA_BCK 13
// EC11 Rotary Encoder
#define PIN_RE_S1 17
#define PIN_RE_S2 16
#define PIN_RE_BTN 4

// ************************
// web server test
// ************************

Carbon CarbonTime;
FileSystem SDCard(5);
AsyncWebServer server(80);

// Replace with your network credentials
const char* ssid     = "research";
const char* password = "Skills39";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to save current epoch time
unsigned long epochTime; 


// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

class Clock
{
  
};

class StatesLED
{
  
};

class BZ
{

};

class Audio
{
public:
  int vol;
};

class Mic1 : public Audio
{

};

class Mic2 : public Audio
{

};

class AUX : public Audio
{

};

class ClockAudio : public Audio
{

};

class UPS
{
  
};

//   // server.on("/sd-card", HTTP_GET, SDCard::api_getList);
//   // server.on("/sd-card/file", HTTP_POST, SDCard::api_uploadFile);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  initWiFi();
  SDCard.init();
  CarbonTime.init();
  
  server.on("/blabla", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse * response = request->beginResponse(200, "text/plain", "Ok");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/json", HTTP_POST, [](AsyncWebServerRequest * request) {
    String content = "{\"success\": false, \"message\": \"blabla\", \"data\": {}}";
    AsyncWebServerResponse * res = request->beginResponse(200, "application/json", content);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    request->send(res);
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String responseContent = SDCard.readTextFile("/index.html");
    request->send(200, "text/html", responseContent);
  });
  server.begin(); //启动服务器
}

void loop() {
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
}