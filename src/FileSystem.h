#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#pragma execution_character_set("utf-8")
#include <Arduino.h>
#include <Vector.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <vfs_api.h>
// JSON support
#include <ArduinoJson.h>
// Network, Web Service
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class FileSystem
{
public:
  FileSystem() { csPin = 5; };
  FileSystem(int _csPin) : csPin(_csPin)
  {}
  bool init() {
    return card.begin(csPin);
  };
  uint8_t cardType();
  uint64_t cardSize();
  
  // [ok] listDir
  DynamicJsonDocument listDir(const char*, uint8_t);
  DynamicJsonDocument listDir(const String, u_int8_t);
  DynamicJsonDocument listDir(const String);
  void api_listDir(AsyncWebServerRequest*);

  // [ok] createDir
  String createDir(const char*);
  String createDir(const String);
  void api_createDir(AsyncWebServerRequest*);

  // renameDir
  void api_renameDir(AsyncWebServerRequest*);

  // [ok] deleteDir
  String deleteDir(const char*);
  String deleteDir(String);
  void api_deleteDir(AsyncWebServerRequest*);
  
  // uploadFile
  void api_uploadFile(AsyncWebServerRequest*);

  // [ok] renameFile
  String renameFile(const char*, const char*);
  String renameFile(String, String);
  void api_renameFile(AsyncWebServerRequest*);

  // [ok] deleteFile
  String deleteFile(const char*);
  String deleteFile(String);
  void api_deleteFile(AsyncWebServerRequest*);
  
  // readFile
  String readTextFile(const char*);
  String readTextFile(const String);

  // append a new file
  String appendFile(const char*, const char*);

  // writeFile
  String writeFile(const char*, const char*);
  
  // for test & debug
  void testFileIO(const char*);

  // [ok] open a file
  File open(const char*); // show be fix
  void api_fileOpen(AsyncWebServerRequest*);
  
  // [ok] get web depends file
  void api_webuiDependsFile(AsyncWebServerRequest*);
private:
  int csPin;
  Vector<String> split(const char, String);
  String previousDirPath(String);
  SDFS card = SDFS(FSImplPtr(new VFSImpl()));
};

#endif // FILESYSTEM_H
