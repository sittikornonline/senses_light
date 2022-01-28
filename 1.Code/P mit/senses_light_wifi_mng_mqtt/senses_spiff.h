#ifndef senses_spiff_h
#define senses_spiff_h
#include "Arduino.h"

#include "FS.h"
#include "SPIFFS.h"
#include "StringSplitter.h"

#define FORMAT_SPIFFS_IF_FAILED true
#define sizeBuffer 1000


class senses_spiff {

  public:
    String readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);
    void deleteFile(fs::FS &fs, const char * path);  
    void writeShitf(String dataFilename);  

    private:
    


};
#endif
