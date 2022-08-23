//**********************************************************************************************************
//*    audioI2S-- I2S audiodecoder for ESP32,  SdFat example                                                             *
//**********************************************************************************************************
//
// first release on 05/2020
// updated on Sep. 27, 2021
/*
 ⒈ install SdFat V2 from https://github.com/greiman/SdFat
 ⒉ activate "SDFATFS_USED"                   in Audio.h
 ⒊ activate "#define USE_UTF8_LONG_NAMES 1"  in SdFatConfig.h
*/

#include <Arduino.h>
#include "WiFi.h"
#include <Audio.h>
#include <SPI.h>
// Digital I/O used
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

#define I2S_DOUT      16
#define I2S_BCLK      4
#define I2S_LRC       17

Audio audio;
// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

String ssid =     "research";
String password = "Skills39";


void setup() {
    pinMode(SD_CS, OUTPUT);      
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SPI.setFrequency(1000000);
    Serial.begin(115200);
    SD.begin(SD_CS);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    delay(3000);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(20); // 0...21
    // audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
    audio.connecttoFS(SD, "ProDer.mp3");
}

void tcr1s(){
    uint32_t act=audio.getAudioCurrentTime();
    uint32_t afd=audio.getAudioFileDuration();
    uint32_t pos =audio.getFilePos();
    Serial.printf("pos =%i\n", pos);
    Serial.printf("audioTime: %i:%02d - duration: %i:%02d\n", (act/60), (act%60) , (afd/60), (afd%60));
}

void loop()
{   
    audio.loop();
    
    if ((millis() - lastTime) > timerDelay) {
        lastTime = millis();
        tcr1s();
    }
}

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}