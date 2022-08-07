#include <Arduino.h>
#include <Configure.h>
// Network, Web Service
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
// Time
#include <Carbon.h>
// I2S Audio
#include <Audio.h>
// SD Card
#include <FileSystem.h>
// Relay Control
#include <util/Relay.h>
// JSON support
#include <ArduinoJson.h>
// MCP4XXXX Chip
#include <util/MCP4XXXX.h>

unsigned long previousMillis = 0;
unsigned long interval = 30000;

// ************************
// web server test
// ************************
Carbon DateTime;
FileSystem SDCard(5);
AsyncWebServer Server(80);
DNSServer DNS;

const char* ssid     = "research";
const char* password = "Skills39";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to save current epoch time
unsigned long epochTime; 


// Initialize WiFi
IPAddress initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return WiFi.localIP();
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

class UPS
{
  
};

void setup() {
  Serial.begin(115200);

  // init
  IPAddress ip = initWiFi();
  DNS.start(DNS_PORT, "blaclock.net", ip);
  SDCard.init();
  DateTime.init();

  // list json in serial port
  String jsonStr;
  serializeJson(SDCard.listDir("/", 0), jsonStr);
  Serial.println(jsonStr);
  
  Server.on("/dir", HTTP_GET, [](AsyncWebServerRequest * req) { SDCard.api_listDir(req); });

  Server.on("/blabla", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse * response = request->beginResponse(200, "text/plain", "Ok");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  Server.on("/json", HTTP_POST, [](AsyncWebServerRequest * request) {
    String content = "{\"success\": false, \"message\": \"blabla\", \"data\": {}}";
    AsyncWebServerResponse * res = request->beginResponse(200, "application/json", content);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    request->send(res);
  });
  Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String responseContent = SDCard.readTextFile("/index.html");
    request->send(200, "text/html", responseContent);
  });
  Server.begin();
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