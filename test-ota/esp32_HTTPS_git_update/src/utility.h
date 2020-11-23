#ifndef utility_H
#define utility_H
#include <main.h>
#include <EEPROM.h>
#include <Wire.h>
#include <StringSplitter.h>
#include <functional>

#define EEPROM_SIZE         128
#define chkSumSaveID_index  38

class TickTask{
    unsigned long Millis_t;
    unsigned long MillisDelay;
    unsigned long MillisCount;
    public: 
        TickTask(unsigned long milDelay);
        bool Update();
        unsigned long getTick();
};

class LEDBlink{

  int ledPin;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time

  bool onLED;
  bool ledBlinkState;
  unsigned long LEDOnDelay;
  unsigned long blinkMillis;

  int ledState;             		// ledState used to set the LED
  unsigned long previousMillis;  	// will store last time LED was updated

 
  public:
  
  LEDBlink(int pin);
  void ledblink(long timeBlink);
  void ledblinkRun();
  
  void ledUpdate(long on, long off);
  void ledON();
  void ledStop(){ OnTime = 0 ;}
};



unsigned int byteArrayToUint32(char* src);
unsigned short byteArrayToUint16(char* src);
unsigned int hexToDec(String hexString);
byte decToBcd(byte val);

uint16_t make16(char x, char y);
uint32_t make32(uint16_t x, uint16_t y);
char gen_checksum(char* str);
String EEPROM_readString(int index, int length);
int EEPROM_writeString(int index, String text);
void EEPROM_writeChkSumID(char ch);
char EEPROM_readChkSumID();
int SearchIndex(String raw, char index);
char * StingTochar(String str);
void printDebug(String s);
void printlnDebug(String s);
uint8_t checkI2C(byte addr);
void scanI2C_device();
char* FlaotTocharArry( float f);

  // #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_LEDBlink)
  // extern LEDBlink led;
  // #endif

#endif