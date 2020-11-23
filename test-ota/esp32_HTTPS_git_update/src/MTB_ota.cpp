#include <MTB_ota.h>

TickTask checkUpdateTick(reCheckTime);
TickTask autoUpdateTick(10000L);

String FirmwareVer    = String(FW_version);
String URL_fw_Version = String(URL_fw_Ver);


const char *rootCACertificate =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
    "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
    "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
    "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
    "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
    "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
    "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
    "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
    "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
    "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
    "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
    "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
    "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
    "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
    "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
    "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
    "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
    "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
    "+OkuE6N36B9K\n"
    "-----END CERTIFICATE-----\n";

struct Button
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button_boot = {
    0,
    0,
    false};

/*void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}*/

void IRAM_ATTR isr()
{
  button_boot.numberKeyPresses += 1;
  button_boot.pressed = true;
}

void init_bootKey(){
  pinMode(button_boot.PIN, INPUT);
  attachInterrupt(button_boot.PIN, isr, RISING);
}


void firmwareUpdate(void)
{
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  //ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  t_httpUpdate_return ret = ESPhttpUpdate.update(URL_fw_Bin);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

int FirmwareVersionCheck(void)
{
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.printf("\nCheck new firmware every %lu sec: ",checkUpdateTick.getTick());
  Serial.println(fwurl);
  WiFiClientSecure *client = new WiFiClientSecure;

  if (client)
  {
    client->setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin(*client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      httpCode = https.GET();

      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
        Serial.printf("  ->code:%d, payload:",httpCode);
        Serial.println(payload);
      }
      else
      {
        Serial.print("  ->error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;

    if (httpCode == HTTP_CODE_OK) // if version received
    {
      payload.trim();
      if (payload.equals(FirmwareVer))
      {
        Serial.print("  ->Device already on latest firmware version: ");
        Serial.println(FirmwareVer);
        Serial.println();
        return 0;
      }
      else
      {

        Serial.print("  ->New firmware detected: ");
        Serial.println(payload);
        return 1;
      }
    }
    
  }
}

int FirmwareAutoUpdate(void)
{
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_autoUpdate;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure *client = new WiFiClientSecure;

  if (client)
  {
    client->setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin(*client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      httpCode = https.GET();

      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
        Serial.printf("  ->code:%d, payload:",httpCode);
        Serial.println(payload);


        const size_t bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
        //const size_t bufferSize = JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 390;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject &JSONencoder = jsonBuffer.parseObject(payload);

        String fwversion  = JSONencoder["esp32httpsOTA"]["fwversion"].as<char *>();
        //String uSens_RevA     = JSONencoder["uSens_Rev.A"].as<char *>();
  
        // Serial.print("  ->esp32httpsOTA : ");   Serial.println(esp32httpsOTA);
        // Serial.print("  ->uSens_Rev.A : ");     Serial.println(uSens_RevA);

         Serial.print("  ->[esp32httpsOTA][fwversion] : ");   Serial.println(fwversion); 
        // Serial.print("  ->type : ");        Serial.println(type); 
        // Serial.print("  ->Signature : ");   Serial.println(Signature); 
        // Serial.print("  ->fwurl : ");       Serial.println(fwurl); 

      }
      else
      {
        Serial.print("  ->error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;

    // if (httpCode == HTTP_CODE_OK) // if version received
    // {
    //   payload.trim();
    //   if (payload.equals(FirmwareVer))
    //   {
    //     Serial.print("  ->Device already on latest firmware version: ");
    //     Serial.println(FirmwareVer);
    //     Serial.println();
    //     return 0;
    //   }
    //   else
    //   {

    //     Serial.print("  ->New firmware detected: ");
    //     Serial.println(payload);
    //     return 1;
    //   }
    // }
    
  }
}


void OTArepeatedCall()
{

  if (button_boot.pressed)
  { //to connect wifi via Android esp touch app
    Serial.println("Firmware update Starting..");
    firmwareUpdate();
    button_boot.pressed = false;
  }

  if(checkUpdateTick.Update()){
    if (FirmwareVersionCheck())
    {
      firmwareUpdate();
    }
  }

  if(autoUpdateTick.Update()){
      FirmwareAutoUpdate();
  }
}