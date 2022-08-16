#include <Arduino.h>
#include <FileSystem.h>
#include <WiFi.h>
#include "time.h"

// Replace with your network credentials
const char* ssid     = "research";
const char* password = "Skills39";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

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


FileSystem SDCard(5);

void setup(){
  Serial.begin(115200);
  initWiFi();
  configTime(0, 0, ntpServer);

  if (! SDCard.init()) {
    Serial.println("Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SDCard.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  
  File file = SDCard.open("/data.txt");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    SDCard.writeFile("/data.txt", "Epoch Time, Temperature, Humidity, Pressure \r\n");
  }
  else {
    Serial.println("File already exists");  
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SDCard.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  SDCard.listDir("/", 0);
  // SDCard.createDir("/blablaDir");
  // // createDir("/mydir");
  // SDCard.listDir("/", 0);
  // // removeDir("/mydir");
  // SDCard.listDir("/", 2);
  // SDCard.writeFile("/hello.txt", "\n\nHello ");
  // SDCard.appendFile("/hello.txt", "World! blablabla\n\n");
  // SDCard.readFile("/hello.txt");
  // SDCard.deleteFile("/foo.txt");
  // SDCard.renameFile("/hello.txt", "/foo.txt");
  // SDCard.readFile("/foo.txt");
  // SDCard.testFileIO("/test.txt");
  // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    //Get epoch time
    epochTime = getTime();

    String dataMessage = String(epochTime) + "\r\n";
    Serial.print("Saving data: ");
    Serial.println(dataMessage);

    //Append the data to file
    SDCard.appendFile("/data.txt", dataMessage.c_str());
    SDCard.readFile("/data.txt");
    
    lastTime = millis();
  }
}