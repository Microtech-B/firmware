#include <main.h>

#define reCheckTime   60000L

#define URL_fw_Bin          "https://raw.githubusercontent.com/Microtech-B/firmware/main/test-ota/auto_update/firmware.bin"
#define URL_fw_Ver          "https://raw.githubusercontent.com/Microtech-B/firmware/main/test-ota/auto_update/fw_version.txt"

#define URL_autoUpdate      "https://raw.githubusercontent.com/Microtech-B/firmware/main/auto_configOTA.txt"

void IRAM_ATTR isr();
void init_bootKey();
void firmwareUpdate(void);
int FirmwareVersionCheck(void);
int FirmwareAutoUpdate(void);
void OTArepeatedCall();
