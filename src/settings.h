#pragma once
// Commentare questa linea per disabilitare le Serial.print
#define DEBUG 1

#define WIFI_MODE
// #define BLE_MODE

//  INIZIO - Definizioni per la ricerca di reti
// Definisce se il modulo deve cercare una rete specifica e basta
#define WIFI_SPECIFIC_LOOKUP 0
// Definisce il nome della rete specifica da cercare nel caso di WIFI_SPECIFIC_LOOKUP = 1
#define SEARCH_WIFI_NAME "PROVA"
// Definisce di tempo tra una scansione e l'altra in millisecondi
#define WIFI_SCAN_PERIOD 2000
// Imposta se la scansione è asincrona o no (Con scansone Asincrona i tempi migliorano apparentemente)
#define ASYNC 0
// Imposta il canale da scansionare
#define CHANNEL 8U
// Imposta i millisecondi massimi per canale
#define MS_PER_CHANNEL 100U
// FINE

// INIZIO - Definizioni per la connessione alla rete madre
// Nome della rete a cui connettersi
#define NETWORK_SSID "IPS_Network"
// Password della rete a cui connettersi
#define NETWORK_PSW "ips_sys@32"
// Host a cui inizializzare il client
#define HOST_IP "192.168.0.2"
// porta dell'host a cui comunicare
#define HOST_PORT 2522
// Definisco la porta su cui ascolto per comunicazioni da parte del server
#define COMM_PORT 100
// FINE

// INIZIO - Definizioni proprie singola antenna
// ID dell'antenna
#define ID 0
// FINE

// INIZO - Codici di comunicazione
// Codice di inizio comunicazione antenna (seguito da ID Antenna e Porta di lettura)
#define SEND_INFO 200
/**
 * L'antenna comunica che è stata disconnessa dalla rete e non ha inviato i dati e li
 * invia in una volta sola
 */
#define SEND_BUFFER_DISCONNECTED 202
// Codice che precede la lista ti tags
#define SEND_TAGS 203
/**
 *  @brief Codice di richiesta di sincronizzazione
 *- l'antenna si mette in ascolto sulla porta comunicata in fase di inizio
 */
#define REQUEST_SYNC server_request_code::SYNC
/**
 * @brief Codice di richiesta di invio della scansione
 *
 */
#define REQUEST_TAGS server_request_code::TAGS

/**
 * @brief Codice di richiesta del server per mettersi in deep sleep
 *
 */
#define REQUEST_DEEP_SLEEP server_request_code::DEEP_SLEEP

/**
 * @brief Codice di richiesta del server per spegnere il client.
 *
 */
#define REQUEST_STOP server_request_code::STOP
// FINE

// Non usate le parti BLUETOOTH
// Periodo di scan in secondi del bluetooth
#define BLE_SCAN_PERIOD 2
// Nome del dispositivo bluetooth
#define DEVICE_NAME "ESP_SERVER_" + ID
// Definisci gli UUID
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
// Numero massimo di dispositivi rintracciabili
#define MAX_ITEM_COUNT 20

enum server_request_code
{
    VOID = -1,
    NO_REQ = 0,
    SYNC = 220,
    STOP = 255,
    TAGS = 221,
    DEEP_SLEEP = 254
};