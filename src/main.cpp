#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
// TODO: SDCard 與 MCP41010共用SPI, modify: util/MCP4XXXX
#include <FileSystem.h>
#include <util/ArxContainer.h>
#include <util/Relay.h>

WebServer server(80);

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

API::API() {
  // route register
  // sd-card
  // server.on("/sd-card", HTTP_GET, SDCard::api_getList);
  // server.on("/sd-card/file", HTTP_POST, SDCard::api_uploadFile);
  // server.on("/sd-card/file", HTTP_PUT, SDCard::api_renameFile);
  // server.on("/sd-card/file", HTTP_DELETE, SDCard::api_deleteFile);
  // server.on("/sd-card/dir", HTTP_POST, SDCard::api_createDir);
  // server.on("/sd-card/dir", HTTP_PUT, SDCard::api_renameDir);
  // server.on("/sd-card/dir", HTTP_DELETE, SDCard::api_deleteDir);
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}