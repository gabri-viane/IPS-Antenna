#include "WifiModule.h"

namespace Module
{
    unsigned long int now;
    unsigned long int last = 0;

    void setupModule()
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        setCpuFrequencyMhz(240);
    }

#if ASYNC
    // Modalità asincrona
    void scanNetwork(AntennaClient *antenna)
    {
        // Imposto i millisecondi correnti
        Module::now = millis();
        // Calcolo il tempo trascorso da inizio scansione
        long int _t = Module::now - Module::last;
        // Prendo il risultato della scansione
        int scan_result = WiFi.scanComplete();
        bool force_scan = false;
        if (_t > WIFI_SCAN_PERIOD)
        {
            if (scan_result > 0)
            {
#ifdef DEBUG
                Serial.printf("- Found %d networks in %d ms\n", scan_result, _t);
#endif
                // Creo array di tag da mandare al server
                unsigned char data[scan_result * 2];
                for (int i = 0, z = 0; i < scan_result; i++)
                {
                    String ssid = WiFi.SSID(i);         // Nome Rete
                    unsigned char rssi = -WiFi.RSSI(i); // Potenza segnale
#ifdef DEBUG
                    Serial.printf("%s : -%d\n", ssid, rssi);
#endif
                    data[z] = ssid[0]; // ID Tag
                    z++;
                    data[z] = rssi; // Segnale TAG
                    z++;
                }
                // Invio il pacchetto di dati al server
                antenna->sendPacket(scan_result, data);
                force_scan = true;
            }
            // se sono in scansion e il tempo trascorso è più di quello impostato
            else if (scan_result == -1 && _t > (WIFI_SCAN_PERIOD + 100))
            {
                // Segna ultima scansione come questa
                Module::last = Module::now;
#ifdef DEBUG
                Serial.printf("- out of scan period (%d,%d)\n", _t, scan_result);
#endif
                // Cancella la scansione corrente per permetterne una nuova
                WiFi.scanDelete();
                force_scan = true;
            }
        }
        // Altrimenti se non è iniziata la scansione o per altri motivi ed è passato troppo tempo
        if (scan_result == -2 || force_scan)
        {
#ifdef DEBUG
            Serial.printf("\nSCAN(%d)... ", scan_result);
#endif
            // Segna ultima scansione come questa
            Module::last = Module::now;
            // Cancella la scansione corrente prima di iniziare la successiva
            WiFi.scanDelete();
            // Avvia una nuova scansione
#if WIFI_SPECIFIC_LOOKUP
            // Scansione di WiFi con nome
            WiFi.scanNetworks(true, true, false, 300U, 0U, SEARCH_WIFI_NAME);
#else
            // Cerco tutte le WiFi
            WiFi.scanNetworks(true, true, false, 300U);
#endif
        }
    }
#else
    // Modalità sincrona
    void scanNetwork(AntennaClient *antenna)
    {
        Module::last = millis();
        int scan_result;
#ifdef DEBUG
        Serial.print("Inizio... ");
#endif
#if WIFI_SPECIFIC_LOOKUP
        scan_result = WiFi.scanNetworks(false, true, false, 300U, 0U, SEARCH_WIFI_NAME);
#else
        scan_result = WiFi.scanNetworks(false, false, false, 100U, 8U);
#endif
#ifdef DEBUG
        Serial.print(" - Fine scansione\n");
#endif
        if (scan_result >= 0)
        {
            unsigned int _t = millis() - Module::last;
#ifdef DEBUG
            Serial.printf("Found %d networks in %d ms\n", scan_result, _t);
#endif
            unsigned char data[scan_result * 2];
            for (int i = 0, z = 0; i < scan_result; i++)
            {
                String ssid = WiFi.SSID(i);
                unsigned char rssi = -WiFi.RSSI(i);
#ifdef DEBUG
                Serial.printf("%s : -%d\n", ssid, rssi);
#endif
                data[z] = ssid[0];
                z++;
                data[z] = rssi;
                z++;
            }
            // Invio il pacchetto di dati al server
            antenna->sendTagsPacket(scan_result, data);
        }
        WiFi.scanDelete();
    }
#endif

    AntennaClient::AntennaClient()
    {
        client = new WiFiUDP();
    }

    void AntennaClient::start()
    {
        while (!this->connectToNetwork())
        {
            delay(10);
        }
    }

    bool AntennaClient::connectToNetwork()
    {
        if (!this->conn_req && !WiFi.isConnected())
        {
            WiFi.begin(NETWORK_SSID, NETWORK_PSW, 8);
            this->conn_req = true;
            this->client->begin(HOST_PORT);
        }
        switch (WiFi.status())
        {
        case wl_status_t::WL_CONNECTED:
            if (this->conn_req)
            {
#ifdef DEBUG
                Serial.println("Connesso");
#endif
                this->conn_req = false;
            }
            return true;
        case wl_status_t::WL_CONNECT_FAILED:
#ifdef DEBUG
            Serial.println("Connessione fallita");
#endif
            this->conn_req = false;
            return true;
        default:
            return false;
        }
    }

    void AntennaClient::sendTagsPacket(unsigned char tag_size, unsigned char data[])
    {
        this->client->beginPacket(HOST_IP, HOST_PORT);
        unsigned char buffer[] = {ID, tag_size};
        this->client->write(buffer, 2);
        this->client->write(data, tag_size * 2);
        this->client->endPacket();
    }

    void AntennaClient::comunicateInfo()
    {
        this->client->beginPacket(HOST_IP, HOST_PORT);
        unsigned char buffer[] = {ID, COMM_PORT};
        unsigned char buffer_2[4];
        // Invio i dati al server per farmi riconoscere
        this->client->write(buffer, 2);
        unsigned long now = htonl(millis() + 10); // Prevedo un ritardo di calcoli e comunicazione di 10ms
        buffer[0] = now << 4;
        this->client->write(now);
    }

}
