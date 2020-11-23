#include <MTB_ota.h>

TickTask printVerTick(1000L);

char ssid[20] = "MTB_tp";
char password[20] = "1234567890mt";
int countTick;


void connect_wifi()
{
  Serial.println("Waiting for WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Active firmware version:");
  Serial.println(FW_version);
  pinMode(LED_BUILTIN, OUTPUT);
  connect_wifi();
  if (FirmwareVersionCheck()){
      firmwareUpdate();
    }
}
void loop()
{
  if(printVerTick.Update()){
    Serial.printf("-> [%d]Active fw version: %s, Build ver: %s\n", countTick++, FW_version,Build_version);
  }
  repeatedCall();
}
