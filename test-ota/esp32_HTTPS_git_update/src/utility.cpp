#include "utility.h"

char dbuf[256];

unsigned int byteArrayToUint32(char* src){
	return (src[0]*256*256*256)+(src[1]*256*256)+(src[2]*256) +src[3];			
}

unsigned short byteArrayToUint16(char* src){
	return (src[0]*256) +src[1];			
}

unsigned int hexToDec(String hexString) {
  unsigned int decValue = 0;
  int nextInt = 0;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}

byte decToBcd(byte val)
{
  return (( (val / 10) * 16) + (val % 10) );
}

uint16_t make16(char x, char y) {
  return ((x << 8) | y);
}

uint32_t make32(uint16_t x, uint16_t y) {
  return ((x << 16) | y);
}

char gen_checksum(char* str)
{
	// between $ and * include $
	unsigned char checksum = 0;
	int i = 0;

	while(((*str) != 0) && ((*str) != '*') && i <2000) 
	{
		//if((*str) != '$')
		checksum ^= (*str++); 
		i++;
	}	
	return checksum;
}

char* FlaotTocharArry( float f){  
    sprintf( dbuf,"%.2f", f);
    return  dbuf;
}

int SearchIndex(String raw, char index){
    char buf[256];
    char sumIndex = 0;
    raw.toCharArray(buf,raw.length()+1);
    for(int i=0;i<raw.length()+2;i++){
        if(buf[i]==index){
            sumIndex++;
        }
    }
    return sumIndex;
}

char * StingTochar(String str){
  str.toCharArray(dbuf,str.length()+1);
  return dbuf;
}

String* spliteString(String str, char split){
    String*item = new String[3];
    int limit = SearchIndex(str, split);
    StringSplitter *splitter = new StringSplitter(str, split, limit);  // new StringSplitter(string_to_split, delimiter, limit)
    int itemCount = splitter->getItemCount();
    for(int i = 0; i < itemCount; i++){
        item[i] = splitter->getItemAtIndex(i);
    }
  return item;
}

String EEPROM_readString(int index, int length) {
  char text[30];
    for(byte i = index ; i < length ; i ++){
        text[i] = EEPROM.read(i);
        text[i+1] = '\0';
        //Serial.printf("%c",EEPROM.read(i));
    }
  return text;
}

int EEPROM_writeString(int index, String text) {
  for (int i = index; i < text.length() + index; ++i) {
    EEPROM.write(i, text[i - index]);
    //Serial.println(text[i-index]);
  }
  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();

  return text.length() + 1;
}

void EEpromWriteInt(int add, int num) {
  int n = num;
  EEPROM.write(add, n >> 8); // เขียนข้อมูลบิตที่ 16 - 9
  EEPROM.write(add + 1, n & 0xFF); // เขียนข้อมูลบิตที่ 8 - 1
  EEPROM.commit();
}

int EEpromReadInt(int add) {
  return make16(EEPROM.read(add), EEPROM.read(add + 1));
}

void EEPROM_writeChkSumID(char ch){
  EEPROM.write(chkSumSaveID_index, ch);
  EEPROM.commit();
}

char EEPROM_readChkSumID(){
  return EEPROM.read(chkSumSaveID_index);
}


void printDebug(String s){
  #if DebugPWR == Debug_on
      Serial.print(s);
  #endif
}

void printlnDebug(String s){
  #if DebugPWR == Debug_on
    Serial.println(s);
  #endif
}


uint8_t checkI2C(byte addr){
  byte error; //variable for error and I2C address
  
  Wire.beginTransmission(addr);
  error = Wire.endTransmission();
  return error;
}


void scanI2C_device(){
  int nDevices = 0;
  byte address ,error;
  for (address = 1; address < 127; address++ ){
    error = checkI2C(address);
    if(error == 0){
      Serial.print("-> I2C device found at address 0x");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }else if (error == 4){
      Serial.print("-> Unknown error at address 0x");
      if (address < 16){
        Serial.print("0");
        Serial.println(address, HEX);
      }
    }
  }
  if (nDevices == 0){
      Serial.println("No I2C devices found\n");
  }  else {
    Serial.print("-> Total i2c devices : [ ");
    Serial.print(nDevices);
    Serial.println(" ] ! \n");
  }
}


//======================== Tick Task =============================

bool TickTask::Update(){
  unsigned long currentMillis = millis();
  MillisCount = currentMillis - Millis_t;
  if (MillisCount >= MillisDelay) {
    Millis_t = currentMillis;
    return true;
  }else return false;
}


unsigned long TickTask::getTick(){
    return MillisCount;
}

TickTask::TickTask(unsigned long milDelay){
    MillisDelay = milDelay;
}

//======================== LED Blink =============================
 LEDBlink::LEDBlink(int pin){
	ledPin = pin;
	pinMode(ledPin, OUTPUT);     
	  
	ledState = LOW; 
	previousMillis = 0;
}

 void LEDBlink::ledON(){
    OnTime = 0;
    digitalWrite(ledPin, 1);
 }
 
 void LEDBlink::ledblink(long timeBlink) {
    onLED = true;
    LEDOnDelay = timeBlink;
 }

 void LEDBlink::ledblinkRun() {
  unsigned long currentMillis = millis();

  if (onLED == true) {
    blinkMillis = currentMillis;
    ledBlinkState = true;
    onLED = false;
  }

  if (ledBlinkState == true) {
    digitalWrite(ledPin, HIGH);

    if ((unsigned long)(currentMillis - blinkMillis) >= LEDOnDelay) {
      digitalWrite(ledPin, LOW);
      ledBlinkState = false;
    }
  }
}

  void LEDBlink::ledUpdate(long on, long off){
    unsigned long currentMillis = millis();
    OnTime = on;
	  OffTime = off;
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }

// #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_LEDBlink)
// LEDBlink led(LED_Status);
// #endif
 
