#ifdef BLUETOOTH
#include "settings.h"
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

namespace Module
{

    typedef struct
    {
        esp_bd_addr_t *address;
        int rssi;
    } BLEScanItem;

    typedef struct
    {
        int size;
        BLEScanItem *items;
    } BLEScanResult;

    enum BLEScanStatus
    {
        SCAN_NULL = -3,
        SCAN_STARTED = -2,
        SCAN_ERROR = -1,
        SCAN_COMPLETED = 0
    };

    class AdvertiserCallback : public BLEAdvertisedDeviceCallbacks
    {
    private:
        BLEScanItem *sr;
        int *index;

    public:
        AdvertiserCallback(BLEScanItem *sr, int *index)
        {
            this->sr = sr;
            this->index = index;
        }

        void onResult(BLEAdvertisedDevice advertisedDevice);
    };

    class BLEScanner
    {
    private:
        BLEScan *pBLEScan;
        AdvertiserCallback *callback;

        BLEScanItem scan_items[MAX_ITEM_COUNT];
        int scan_size;
        BLEScanStatus status = BLEScanStatus::SCAN_NULL;

        unsigned long int scan_start = 0;
        unsigned long int scan_stop = 0;

        void setup();
        void addScanItem(BLEScanItem);
        void resetScan();

    public:
        BLEScanner()
        {
            setup();
        }

        ~BLEScanner()
        {
            this->pBLEScan->clearResults();
            this->pBLEScan->stop();
            BLEDevice::deinit();
            delete this->pBLEScan;
            delete this->callback;
        }

        void scanNetwork();

        BLEScanResult getScanResult();

        void setStatus(BLEScanStatus);

        BLEScanStatus getStatus();

        unsigned long int elapsedScanTime();
    };

    void onComlpeteScan(BLEScanResults);
}
#endif