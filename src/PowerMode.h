#pragma once
#include "settings.h"
#include "WifiModule.h"

namespace Module
{

    class EnergyHandler
    {
    private:
        AntennaClient *antenna;
        void (*callback)();
        unsigned long long int deep_sleep_micros = 0;
        unsigned long int wait_millis = 0;
        bool request_sent = false;
        antenna_status_code current = antenna_status_code::NO_STATUS;

        /**
         * @brief Invia un messaggio al server. Aspetta un risposta per tot tempo e
         * ritorna in deep_sleep se non riceve risposta.
         * Se riceve risposta esce dal ciclo di deep_sleep e chiama il callback.
         *
         */
        void checkStatus();

    public:
        EnergyHandler(Module::AntennaClient *antenna);
        ~EnergyHandler();

        /**
         * @brief Imposta un callback da chiamare quando il server
         * invia un messaggio per uscire dalla deep_sleep
         *
         * @param callback Funzione callback da chiamare
         */
        void setCallback(void (*callback)());

        /**
         * @brief Inizia il ciclo deep_sleep dell'antenna.
         * Ogni tot. viene inviato un messaggio al server e
         * aspetta una risposta per tot secondi, se non la riceve
         * ritorna in deep_sleep
         *
         */
        void start();

        antenna_status_code requestStatus();
    };

    void power_deep_sleep(unsigned long millis);
}
