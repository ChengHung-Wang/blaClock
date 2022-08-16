#include <vfs_api.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
// #include <ArxContainer.h>

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
  void listDir(const char*, uint8_t);
  void createDir(const char*);
  void removeDir(const char*);
  void readFile(const char*);
  void writeFile(const char*, const char*);
  void appendFile(const char*, const char*);
  void renameFile(const char*, const char*);
  void deleteFile(const char*);
  void testFileIO(const char*);
  File open(const char*); // show be fix
private:
  int csPin;
  SDFS card = SDFS(FSImplPtr(new VFSImpl()));
};
