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
// MAX17043
#include <Wire.h> // Needed for I2C
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
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
Audio audio;

// Create a MAX17043
SFE_MAX1704X lipo(MAX1704X_MAX17043);
double voltage = 0; // Variable to keep track of LiPo voltage
double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
bool alert; // Variable to keep track of whether alert has been triggered

String ssid     = "research";
String password = "Skills39";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to save current epoch time
unsigned long epochTime; 

// Initialize WiFi
IPAddress initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return WiFi.localIP();
}

void i2cScan() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // 0x32
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      Serial.println(String("address: ") + address);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void setup() {
  Serial.begin(115200);
  
  // init
  IPAddress ip = initWiFi();
  DNS.start(DNS_PORT, "blaclock.net", ip);
  SDCard.init();
  DateTime.init();
  Wire.begin(PIN_SDA, PIN_SCL);

  // MAX1704
  // Wire.begin(0x32, PIN_SDA, PIN_SCL);
  lipo.enableDebugging();
  // Set up the MAX17044 LiPo fuel gauge:
  if (lipo.begin(Wire) == false) // Connect to the MAX17044 using the default wire port
  {
    Serial.println(F("MAX17044 not detected. Please check wiring. Freezing."));
    // while (1);
    lipo.quickStart();
	  lipo.setThreshold(20); // Set alert threshold to 20%.
  }

  // ----------------------
  // Web Service
  // ----------------------
  Server.on("/api/v1/dir", HTTP_GET, [](AsyncWebServerRequest * req) { SDCard.api_listDir(req); });
  Server.on("/api/v1/dir", HTTP_POST, [](AsyncWebServerRequest * req) { SDCard.api_createDir(req); });
  
  Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    File responseContent = SDCard.open("/dist/index.html");
    AsyncWebServerResponse *res = request->beginResponse(responseContent, "/", "text/html", false);
    res->addHeader("charset", "UTF-8");
    res->setCode(200);
    request->send(res);
  });
  Server.onNotFound([](AsyncWebServerRequest *request) {
    String url = request->url();
    File targetFile = SDCard.open(("/dist" + url).c_str());
    // ex: 10.71.74.15/favicon.ico => url = /favicon.ico
    if (targetFile) {
      AsyncWebServerResponse *res = request->beginResponse(targetFile, url, "", false);
      res->setCode(200);
      request->send(res);
    }else {
      String jsonStr;
      DynamicJsonDocument result(4096);
      result["success"] = false;
      result["message"] = "ERR_URL_NOT_FOUND";
      result["data"] = "";
      serializeJson(result, jsonStr);
      AsyncWebServerResponse *res = request->beginResponse(404, "application/json", jsonStr);
      request->send(res);
    }
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
  // audio.loop();

  // voltage = lipo.getVoltage();
	// // lipo.getSOC() returns the estimated state of charge (e.g. 79%)
	// soc = lipo.getSOC();
	// // lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
	// alert = lipo.getAlert();

	// // Print the variables:
	// Serial.print("Voltage: ");
	// Serial.print(voltage);  // Print the battery voltage
	// Serial.println(" V");

	// Serial.print("Percentage: ");
	// Serial.print(soc); // Print the battery state of charge
	// Serial.println(" %");
  // delay(5000);
}