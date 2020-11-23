#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include "utility.h"


#define FW_version "0.5"

#define LED_BUILTIN 2