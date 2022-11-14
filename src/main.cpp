#include <Arduino.h>
#include "settings.h"

#ifdef BLE_MODE
#define BLUETOOTH
#include "BLEModule.h"

Module::BLEScanner *scanner;

void setup()
{
  Serial.begin(115200);
  Serial.println("Partito");
  scanner = Module::inst;
  scanner = new Module::BLEScanner();
}

void loop()
{
  scanner->scanNetwork();
  if (scanner->getStatus() != Module::BLEScanStatus::SCAN_STARTED)
  {
    Module::BLEScanResult sr = scanner->getScanResult();
    for (int i = 0; i < sr.size; i++)
    {
      unsigned int *orig = (unsigned int *)(sr.items[i].address);
      Serial.printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\tRSSI:%d\n",
                    orig[0] & 0xff, orig[1] & 0xff, orig[2] & 0xff,
                    orig[3] & 0xff, orig[4] & 0xff, orig[5] & 0xff, sr.items[i].rssi);
    }
  }
}
#endif

#ifdef WIFI_MODE
#define WIFI
#include "WifiModule.h"

Module::AntennaClient *antenna;

void setup()
{
  Serial.begin(115200);
  Module::setupModule();
  antenna = new Module::AntennaClient();
  antenna->start();
}

void loop()
{
  Module::scanNetwork(antenna);
}

#endif