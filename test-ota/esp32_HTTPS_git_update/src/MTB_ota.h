#include <main.h>

#define reCheckTime   60000L

#define URL_autoUpdate      "https://raw.githubusercontent.com/Microtech-B/firmware/main/auto_configOTA.txt"

void IRAM_ATTR isr();
void init_bootKey();
void firmwareUpdate(void);
int FirmwareAutoUpdate(void);
void OTArepeatedCall();
