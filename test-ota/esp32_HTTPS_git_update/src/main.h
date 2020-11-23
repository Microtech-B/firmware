#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <utility.h>
#include <version.h>


#define FW_version  VERSION_DATETIME

#define LED_BUILTIN 2