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
            WiFi.scanNetworks(true, true, false, MS_PER_CHANNEL, CHANNEL, SEARCH_WIFI_NAME);
#else
            // Cerco tutte le WiFi
            WiFi.scanNetworks(true, true, false, MS_PER_CHANNEL, CHANNEL);
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
        scan_result = WiFi.scanNetworks(false, true, false, MS_PER_CHANNEL, CHANNEL, SEARCH_WIFI_NAME);
#else
        scan_result = WiFi.scanNetworks(false, false, false, MS_PER_CHANNEL, CHANNEL);
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
            register unsigned char data[scan_result * 7]; // 7= 6 bytes MAC_Address + 1 RSSI
            register unsigned char *MAC;
            register unsigned char rssi;
            for (int i = 0, z = 0; i < scan_result; i++)
            {
                MAC = WiFi.BSSID(i);
                rssi = -WiFi.RSSI(i);
                data[z++] = MAC[0];
                data[z++] = MAC[1];
                data[z++] = MAC[2];
                data[z++] = MAC[3];
                data[z++] = MAC[4];
                data[z++] = MAC[5];
                data[z++] = rssi;
#ifdef DEBUG
                Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x : -%d\n", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5], rssi);
#endif
            }
            // Invio il pacchetto di dati al server
            antenna->sendTagsPacket(data, scan_result);
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
        this->client->begin(COMM_PORT);
        this->sendInfoPacket();
    }

    void AntennaClient::stop()
    {
        this->client->stop();
    }

    bool AntennaClient::connectToNetwork()
    {
        if (!this->conn_req && !WiFi.isConnected())
        {
            WiFi.begin(NETWORK_SSID, NETWORK_PSW, 8);
            this->conn_req = true;
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

    server_request_code AntennaClient::requestStatus()
    {
        if (this->client->parsePacket() > 0)
        {
            int read = this->client->read();
            return server_request_code(read);
        }
        return server_request_code::VOID;
    }

    void AntennaClient::sendTagsPacket(unsigned char *data, unsigned char tag_size)
    {
        this->client->beginPacket(HOST_IP, HOST_PORT);
        this->tag_buffer[2] = tag_size;
        this->client->write(this->tag_buffer, 3);
        this->client->write(data, tag_size * 2);
        this->client->endPacket();
    }

    void AntennaClient::sendInfoPacket()
    {
        this->client->beginPacket(HOST_IP, HOST_PORT);
        this->client->write(this->info_buffer, 3);
        this->client->endPacket();
    }

    int AntennaClient::getNext()
    {
        int total_chars = this->client->available();
        if (total_chars > 0)
        {
            return this->client->read();
        }
        return 0;
    }

}
