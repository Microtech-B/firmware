#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <ArduinoJson.h>
#include <stdint.h>
#include <utility.h>
#include <version.h>

#define signature       "40622e6d457461615f68747470734f54413d31"
#define Product_Type    "esp32httpsOTA"

//#define FW_version      VERSION_DATETIME
#define FW_version      "20201123_215159"
#define Build_version   VERSION

#define LED_BUILTIN 2