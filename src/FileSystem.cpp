#include "FileSystem.h"

uint8_t FileSystem::cardType() {
  return card.cardType();
}

uint64_t FileSystem::cardSize() {
  return card.cardSize();
}

File FileSystem::open(const char* fileName) {
  return card.open(fileName);
}

void FileSystem::api_fileOpen(AsyncWebServerRequest *req) {
  String jsonStr;
  int responseCode;
  DynamicJsonDocument result(4096);
  if (! req->hasParam("path")) {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";
    responseCode = 400;
  }else {
    String filePath = req->getParam("path")->value();
    File targetFile = this->card.open(filePath);
    if (targetFile) {
      AsyncWebServerResponse *res = req->beginResponse(targetFile, filePath, "", false);
      res->setCode(200);
      req->send(res);
      return;
    }else {
      result["success"] = false;
      result["message"] = "ERR_URL_NOT_FOUND";
      result["data"] = "";
      responseCode = 404;
    }
  }
  serializeJson(result, jsonStr);
  AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
  res->addHeader("Access-Control-Allow-Origin", "*");
  res->addHeader("Accept", "application/json");
  req->send(res);
}

Vector<String> FileSystem::split(const char splitBy, String src) {
  Vector<String> result;
  String cache = "";
  for (const char &item : src) {
    if (item != splitBy) {
      cache += item;
    }else {
      if (cache != "") {
        result.push_back(cache);
      }
      cache = "";
    }
  }
  if (cache != "") {
    result.push_back(cache);
  }
  return result;
}

String FileSystem::previousDirPath(String path) {
  String result = "/";
  Vector<String> pathBreak = split('/', path);
  if (pathBreak.size() > 1) {
    for (int index = 0; index <= pathBreak.size() - 2; index ++) {
      result += pathBreak[index];
      if (index != pathBreak.size() - 2) {
        result += "/";
      }
    }
  }
  return result;
}

// [ok] list dir
DynamicJsonDocument FileSystem::listDir(const char * dirname, uint8_t levels) {
  DynamicJsonDocument result(4096);
  result["dirs"].to<JsonArray>();
  result["files"].to<JsonArray>();
  File root = card.open(dirname);
  if(!root){
    //TODO: save to syslog
    // Serial.println("Failed to open directory");
    return result;
  }
  if(!root.isDirectory()){
    return result;
  }
  File file = root.openNextFile();

  while(file){
    if(file.isDirectory()){
      DynamicJsonDocument thisDir(1024);
      thisDir["name"] = String(file.name());
      thisDir["lastModify"] = file.getLastWrite();
      result["dirs"].add(thisDir);
      if(levels){
        listDir(file.name(), levels - 1);
      }
    } 
    else {
      DynamicJsonDocument thisFile(1024);
      thisFile["name"] = String(file.name());
      thisFile["size"] = file.size();
      thisFile["lastModify"] = file.getLastWrite();
      result["files"].add(thisFile);
    }
    file = root.openNextFile();
  }
  return result;
}

DynamicJsonDocument FileSystem::listDir(String path,uint8_t levels) {
  return listDir(path.c_str(), levels);
}

DynamicJsonDocument FileSystem::listDir(String path) {
  return listDir(path.c_str(), 0);
}

void FileSystem::api_listDir(AsyncWebServerRequest *request) {
  String jsonStr;
  DynamicJsonDocument result(4096);
  if (request->hasParam("path")) {
    String path = request->getParam("path")->value();
    result["success"] = true;
    result["message"] = "";
    result["data"].add(listDir(path.c_str(), 0));

    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = request->beginResponse(200, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    request->send(res);
  }
  else {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";

    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = request->beginResponse(400, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    request->send(res);
  }
}

// [ok] read text file
String FileSystem::readTextFile(const char* path)
{
  String result = "";
  File file = card.open(path);
  if(!file){
    // TODO: save to syslog
    // Serial.println("Failed to open file for reading");
    return result;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    char thisChar = file.read();
    result += String(thisChar);
  }
  file.close();
  return result;
}

String FileSystem::readTextFile(const String path) {
  return FileSystem::readTextFile(path.c_str());
}

// [ok] create dir
String FileSystem::createDir(const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(! card.mkdir(path)){
    return "mkdir failed";
  }
  return "";
}

String FileSystem::createDir(String path){
  return createDir(path.c_str());
}

void FileSystem::api_createDir(AsyncWebServerRequest *req) {
  String jsonStr;
  DynamicJsonDocument result(4096);
  if (req->hasParam("path")) {
    String path = req->getParam("path")->value();
    String mkdirResult = createDir(path);
    int responseCode;
    if (mkdirResult == "") {
      responseCode = 200;
      result["success"] = true;
      result["message"] = "";
      result["data"].add(listDir(path.c_str(), 0));
    }else {
      responseCode = 500;
      result["success"] = false;
      result["message"] = mkdirResult;
      result["data"] = "";
    }
    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
  else {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";

    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(400, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
}

// [ok] remove dir
String FileSystem::deleteDir(const char * path){
  if(! card.rmdir(path)){
    return "rmdir failed";
  }
  return "";
}

String FileSystem::deleteDir(String path) {
  return deleteDir(path.c_str());
}

void FileSystem::api_deleteDir(AsyncWebServerRequest *req) {
  String jsonStr;
  DynamicJsonDocument result(4096);
  if (req->hasParam("path")) {
    String path = req->getParam("path")->value();
    String rmdirResult = deleteDir(path);
    int responseCode;
    if (rmdirResult == "") {
      responseCode = 200;
      result["success"] = true;
      result["message"] = "";
      result["data"].add(listDir(previousDirPath(path)));
    }else {
      responseCode = 500;
      result["success"] = false;
      result["message"] = rmdirResult;
      result["data"] = "";
    }
    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
  else {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";

    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(400, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
}

// 
String FileSystem::writeFile(const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = card.open(path, FILE_WRITE);
  if(!file){
    return "Failed to open file for writing";
  }
  if(! file.print(message)){
    return "Write failed";
  }
  file.close();
  return "";
}

String FileSystem::appendFile(const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);
  File file = card.open(path, FILE_APPEND);
  if(!file){
    return "Failed to open file for appending";
  }
  if(! file.print(message)){
    return "Append failed";
  }
  file.close();
  return "";
}

String FileSystem::renameFile(const char * path1, const char * path2) {
  if (! card.rename(path1, path2)) {
    return "Rename failed";
  }
  return "";
}

String FileSystem::renameFile(String path1, String path2) {
  return this->renameFile(path1.c_str(), path2.c_str());
}

void FileSystem::api_renameFile(AsyncWebServerRequest* req) {
  /************************
   * param callbackPath which dirList location you want return(optional)
   * param path1 srcPath
   * param path2 targetPath
  /************************/
  String jsonStr;
  DynamicJsonDocument result(4096);
  int responseCode;
  if (!req->hasParam("path1") || !req->hasParam("path2")) {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";
    responseCode = 400;
  }else {
    String path1 = req->getParam("path1")->value(), 
           path2 = req->getParam("path2")->value();
    String renameResult = this->renameFile(path1, path2);
    if (renameResult == "") {
      responseCode = 200;
      result["success"] = true;
      result["message"] = "";
      if (req->hasParam("callbackPath")) {
        result["data"] = listDir(req->getParam("callbackPath")->value());
      }else {
        result["data"] = "";
      }
    }else {
      responseCode = 500;
      result["success"] = false;
      result["message"] = renameResult;
      result["data"] = "";
    }
  }
  serializeJson(result, jsonStr);
  AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
  res->addHeader("Access-Control-Allow-Origin", "*");
  res->addHeader("Accept", "application/json");
  req->send(res);
}

String FileSystem::deleteFile(const char * path) {
  if(! card.remove(path)){
    return "Delete failed";
  }
  return "";
}

String FileSystem::deleteFile(String path) {
  return this->deleteFile(path.c_str());
}

void FileSystem::api_deleteFile(AsyncWebServerRequest *req) {
  String jsonStr;
  DynamicJsonDocument result(4096);
  if (req->hasParam("path")) {
    String path = req->getParam("path")->value();
    String rmdirResult = deleteFile(path);
    int responseCode;
    if (rmdirResult == "") {
      responseCode = 200;
      result["success"] = true;
      result["message"] = "";
      result["data"].add(listDir(path));
    }else {
      responseCode = 500;
      result["success"] = false;
      result["message"] = rmdirResult;
      result["data"] = "";
    }
    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(responseCode, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
  else {
    result["success"] = false;
    result["message"] = "ERR_MISSING_FIELD";
    result["data"] = "";

    serializeJson(result, jsonStr);
    AsyncWebServerResponse * res = req->beginResponse(400, "application/json", jsonStr);
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Accept", "application/json");
    req->send(res);
  }
}

void FileSystem::testFileIO(const char * path){
  File file = card.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = card.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void FileSystem::api_webuiDependsFile(AsyncWebServerRequest *request) {
  String url = request->url();
  // ex: 10.71.74.15/favicon.ico => url = /favicon.ico
  File targetFile = this->card.open(("/dist" + url).c_str());
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
}