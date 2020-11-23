#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <utility.h>
#include <version.h>

#define signature      "422e4d4554415f6d7462"
   
#define FW_version      VERSION_DATETIME
#define Build_version   VERSION

#define LED_BUILTIN 2