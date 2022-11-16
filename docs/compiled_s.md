# Impostazioni Antenna compilate

Nome|Tipo|Default|Descrizione
--|--|--|--
DEBUG|int|0|Stampa informazioni di debug
WIFI_MODE|-|attivato|Imposta la modalità a WiFi
BLE_MODE|-|disattivato|Imposta la modalità a BLE
WIFI_SPECIFIC_LOOKUP|bool|0|Cerca per un WiFi specifico
SEARCH_WIFI_NAME|string|"PROVA"|Nome della WiFi specifica da cercare
WIFI_SCAN_PERIOD|int|2000|Intervallo in secondi di ogni scansione se l'antenna perde la connessione al server
ASYNC|bool|0|Imposta la scansione come asincrona
CHANNEL|unsigned int|8U|Canale WiFi da scansionare (utile per ridurre il carico di lavoro)
MS_PER_CHANNEL|unsigned int|100U|Millesecondi di scansione per canale
NETWORK_SSID|string|"IPS_Network"|Nome rete del sistema
NETWORK_PSW|string|"ips_sys@32"|Password della rete del sistema
HOST_IP|string|"192.168.0.2"|Indirizzo IP locale della rete del sistema che rappresenta il server
HOST_PORT|int|2522|Porta aperta del server su cui comunicare
COMM_PORT|int|100|Porta aperta dell'antenna su cui il server peuò comunicare
ID|int|0|ID dell'antenna