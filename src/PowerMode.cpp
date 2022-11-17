#include "PowerMode.h"

namespace Module
{

    EnergyHandler::EnergyHandler(Module::AntennaClient *antenna)
    {
        this->antenna = antenna;
    }

    antenna_status_code EnergyHandler::requestStatus()
    {
        return this->current;
    }

    void EnergyHandler::start()
    {
        this->current = antenna_status_code::IN_DEEP_SLEEP;
        int time_in_minutes = this->antenna->getNext();
        if (time_in_minutes < 1)
        {
            time_in_minutes = 1;
        }
        this->deep_sleep_micros = time_in_minutes * 60000000;
        Module::power_deep_sleep(this->deep_sleep_micros);
    }

    void EnergyHandler::setCallback(void (*v)())
    {
        this->callback = v;
    }

    void EnergyHandler::checkStatus(){
        
    }

    void power_deep_sleep(unsigned long millis)
    {
        esp_deep_sleep(millis * 1000);
    }

}
