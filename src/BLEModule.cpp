#ifdef BLUETOOTH
#include "BLEModule.h"

namespace Module
{
    BLEScanner inst;

    void AdvertiserCallback::onResult(BLEAdvertisedDevice advertisedDevice)
    {
        BLEScanItem si;
        si.address = advertisedDevice.getAddress().getNative();
        si.rssi = advertisedDevice.getRSSI();
        this->sr[*this->index] = si;
        (*this->index)++;
    }

    void BLEScanner::setup()
    {
        BLEDevice::init(DEVICE_NAME);
        pBLEScan = BLEDevice::getScan(); // create new scan
        callback = new AdvertiserCallback(this->scan_items, &this->scan_size);
        pBLEScan->setAdvertisedDeviceCallbacks(callback);
        pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
        pBLEScan->setInterval(100);
        pBLEScan->setWindow(100); // less or equal setInterval value
    }

    void BLEScanner::scanNetwork()
    {
        if (this->status != BLEScanStatus::SCAN_STARTED)
        {
            Serial.println("Scan started");
            this->status = BLEScanStatus::SCAN_STARTED;
            scan_start = millis();
            pBLEScan->clearResults();
            this->resetScan();
            bool res = pBLEScan->start(BLE_SCAN_PERIOD, &Module::onComlpeteScan, true);
            if (!res)
            {
                this->status = BLEScanStatus::SCAN_ERROR;
            }
            Serial.println("Scan finished");
        }
    }

    void BLEScanner::setStatus(BLEScanStatus new_status)
    {
        this->status = new_status;
    }

    BLEScanStatus BLEScanner::getStatus()
    {
        return this->status;
    }

    BLEScanResult BLEScanner::getScanResult()
    {
        return {this->scan_size, this->scan_items};
    }

    unsigned long int BLEScanner::elapsedScanTime()
    {
        return scan_stop - scan_start;
    }

    void BLEScanner::addScanItem(BLEScanItem si)
    {
        this->scan_items[this->scan_size] = si;
        this->scan_size++;
    }

    void BLEScanner::resetScan()
    {
        this->scan_size = 0;
    }

    void onComlpeteScan(BLEScanResults sc)
    {
        Serial.println("Scan completed");
        inst.setStatus(BLEScanStatus::SCAN_COMPLETED);
    }
}
#endif