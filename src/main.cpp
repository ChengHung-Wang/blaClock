#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SD.h> 
#include <SPI.h>

#include <vector>
#include <map>

WebServer server(80);

class SDCard 
{
public:
  std::vector<std::map<String, String>> getList();
  static void api_getList();
  static void api_createDir();
  static void api_renameDir();
  static void api_deleteDir();
  static void api_uploadFile();
  static void api_renameFile();
  static void api_deleteFile();
private:
  String fileSize();
};

std::vector<std::map<String, String>> getList() {
  std::vector<std::map<String, String>> result;
  return result;
}

void SDCard::api_getList() {
  // TODO: 
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

class API
{
public:
  API();
private:
};

API::API() {
  // route register
  // sd-card
  server.on("/sd-card", HTTP_GET, SDCard::api_getList);
  server.on("/sd-card/file", HTTP_POST, SDCard::api_uploadFile);
  server.on("/sd-card/file", HTTP_PUT, SDCard::api_renameFile);
  server.on("/sd-card/file", HTTP_DELETE, SDCard::api_deleteFile);
  server.on("/sd-card/dir", HTTP_POST, SDCard::api_createDir);
  server.on("/sd-card/dir", HTTP_PUT, SDCard::api_renameDir);
  server.on("/sd-card/dir", HTTP_DELETE, SDCard::api_deleteDir);
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}