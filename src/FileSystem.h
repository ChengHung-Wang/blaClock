#include <vfs_api.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Arduino.h>
#include <ArxContainer.h>

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
  arx::vector<String> blabla();
  // arx::map<String, arx::vector<arx::map<String, String>>> listDir(const char*, uint8_t);
  void createDir(const char*);
  void removeDir(const char*);
  String readTextFile(const char*);
  String readTextFile(const String);
  void readFile(const char*);
  void writeFile(const char*, const char*);
  void appendFile(const char*, const char*);
  void renameFile(const char*, const char*);
  void deleteFile(const char*);
  void testFileIO(const char*);
  static void api_getList();
  static void api_createDir();
  static void api_renameDir();
  static void api_deleteDir();
  static void api_uploadFile();
  static void api_renameFile();
  static void api_deleteFile();
  File open(const char*); // show be fix
private:
  int csPin;
  SDFS card = SDFS(FSImplPtr(new VFSImpl()));
};
