#include "settings.h"
#include "WiFi.h"
#include "WiFiUdp.h"

namespace Module
{

    class AntennaClient
    {
    private:
        WiFiUDP *client;
        bool _init = false;

        // Array di comunicazione
        const unsigned char info_buffer[3] = {SEND_INFO, ID, COMM_PORT}; // Inizio trasmissione con il server
        unsigned char tag_buffer[3] = {SEND_TAGS, ID, 0};                // Inizio comunicazione lista tag

        /*
        Indica se è stata inviata una nuova richiesta di connessione
        - true: ha inviato la richiesta per connettersi alla wifi
        - false: ha già chiesto o non ha ancora chiesto

        Per verificare se si è connessi basta fare WiFi.isConnected()
        */
        bool conn_req = false;

        /**
         * @brief Comunica il proprio id antenna, indirizzo ip locale, porta
         * con il server per iniziare la comunicazione.
         *
         */
        void sendInfoPacket();
        /**
         * @brief Si connette alla rete definita nel file "settings.h"
         *
         */
        bool connectToNetwork();

    public:
        /**
         * @brief Construct a new Antenna Client object
         *
         */
        AntennaClient();

        /**
         * @brief Destroy the Antenna Client object. Chiude il client UDP
         * aperto alla creazione di quest'instanza.
         *
         */
        ~AntennaClient()
        {
            delete this->client;
        }

        /**
         * @brief Aspetta di connettersi alla rete specificata nell'header "settings.h"
         *
         * @see settings.h
         */
        void start();

        /**
         * @brief Chiude la connessione al server.
         *
         */
        void stop();

        /**
         * @brief Invia un pacchetto dati al server specificando in automatico il proprio
         * ID di antenna definisto nel file "settings.h" e il numero di tag che sta inviando
         *
         * @param data Array di char contenente i dati da inviare
         * (la dimensione deve essere tag_size * 2)
         * @param tag_size Numero di reti dei tag rilevate
         */
        void sendTagsPacket(unsigned char *data, unsigned char tag_size = 0);

        /**
         * @brief Controlla l'ultimo codice inviato del server e lo ritorna
         *
         * @return server_request_code Il codice ricevuto dal server
         */
        server_request_code requestStatus();
    };

    /**
     * @brief Imposta l'ESP come STA e si disconnette da eventuali
     * reti a cui era precedentemente connesso.
     *
     */
    void setupModule();

    /**
     * @brief Esegue lo scan delle reti wifi su un canale specifico e
     * i risultati vengono cominicati al server impostato nell'header "settings.h"
     *
     * @see settings.h
     *
     * @param antenna Client antenna che permette di comunicare al server.
     * Deve essere già stato chiamato il metodo antenna->start().
     */
    void scanNetwork(AntennaClient *antenna);

}