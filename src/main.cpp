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
#ifdef DEBUG
  Serial.begin(115200);
#else
  Serial.end(true);
#endif
  Module::setupModule();
  antenna = new Module::AntennaClient();
  antenna->start();
}

void loop()
{
  switch (antenna->requestStatus())
  {
  case REQUEST_TAGS:
    Module::scanNetwork(antenna);
    break;
  case REQUEST_STOP:
    antenna->stop();
    break;
  case REQUEST_SYNC:
    antenna->start();
    break;
  case REQUEST_DEEP_SLEEP:
    break;
  case server_request_code::VOID:
  case server_request_code::NO_REQ:
  default:
    break;
  }
}

#endif