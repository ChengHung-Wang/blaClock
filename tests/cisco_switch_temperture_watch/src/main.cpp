#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

// temperture api
String apiURI = "http://10.71.74.6:5487/";

bool alert = false;

// Replace with your network credentials
const char* ssid     = "research";
const char* password = "Skills39";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void checkStatus() {
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(apiURI.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        int temperture = payload.toInt();
        if (temperture >= 49) {
          alert = true;
          Serial.println("alert!!");
        }else {
          alert = false;
        }
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  configTime(0, 0, ntpServer);
  initWiFi();
  pinMode(5, OUTPUT);
}

void loop() {
  if (alert) {
    digitalWrite(5, HIGH);
  }else {
    digitalWrite(5, LOW);
  }
  // put your main code here, to run repeatedly:
  if ((millis() - lastTime) > timerDelay) {
    checkStatus();
  }
}