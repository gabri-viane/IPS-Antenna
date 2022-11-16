[⬅️ Indice](index.md)
# Comunicazione Antenna > Server

In questo file vengono spiegate le risposte dell'antenna al server. Le risposte possono essere causate da una richiesta del server o da un comportamento autonomo quale: 
-   accensione antenna
-   connessione al wifi
-   uscita deep sleep
-   fine richiesta di stop

<p>
L'antenna comunica al server usando sempre come primo byte della stringa inviata un codice che corrisponde al messaggio che vuole comunicare. Di seguito sono elencati i codici possibili che il server può ricevere e sono successivamente spiegate le costruzioni delle risposte.
</p>

### *Tabella Codici*

<table>
<thead>
  <tr>
    <th>Codice</th>
    <th>Operazione</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>200</td>
    <td>SEND_INFO</td>
  </tr>
  <tr>
    <td>202</td>
    <td>SEND_BUFFER_DISCONNECTED</td>
  </tr>
  <tr>
    <td>203</td>
    <td>SEND_TAGS</td>
  </tr>
</tbody>
</table>

> ## Per sapere cosa deve chiedere il server vedi [qui](stoa.md)

#### 1. **SEND_INFO**
L'antenna invia questa risposta in 3 casi diversi:
   1.   L'antenna si è appena connessa alla rete wifi
   2.   Viene richiesto dal serve con il codice *REQUEST_SYNC*
   3.   L'antenna si è disconnessa e riconnessa alla rete wifi

La risposta si compone come stringa di 3 bytes:  

| #Byte | Valore | Significato                                                |
| ----- | ------ | ---------------------------------------------------------- |
| 0     | 200    | SEND_SYNC                                                  |
| 1     | Numero | ID dell'antenna                                            |
| 2     | Numero | Porta dell'antenna aperta alla comunicazione con il server |
   
#### 2. **SEND_BUFFER_DISCONNECTED**--*Non ancora implementato*
Quando l'antenna si disconnette dal WiFi esegue scansioni regolari ogni tot. millisecondi e quando si riconnette al server comunica il risultato di tutte le scansioni. 

#### 3. **SEND_TAGS** 
Risponde al comando *REQUEST_TAGS* inviato dal server: dopo aver eseguito la scansione invia una risposta composta da un header e un corpo contenente la lista di valori dei tags:

1. Header:

    | #Byte | Valore | Significato          |
    | ----- | ------ | -------------------- |
    | 0     | 203    | SEND_TAGS            |
    | 1     | Numero | ID Antenna           |
    | 2     | Numero | Numero tags elencati |

2. Corpo:
 
    Il corpo contiene tanti elementi quanti specificati nel byte 2 dell'header. 
    
    Ogni elemento è costruito in questo modo: i primi 6 bytes sono il MAC-Address del Tag e il settimo byte è il valore assoluto del RSSI:

     | #Byte | Valore            | Significato        |
     | ----- | ----------------- | ------------------ |
     | 0     | Numero            | Primo valore MAC   |
     | 1     | Numero            | Secondo valore MAC |
     | 2     | Numero            | Terzo valore MAC   |
     | 3     | Numero            | Quarto valore MAC  |
     | 4     | Numero            | Quinto valore MAC  |
     | 5     | Numero            | Sesto valore MAC   |
     | 6     | Numero (0 .. 100) | Valore RSSI        |

Un esempio di risposta potrebbe essere

    (char)203 (char)5 (char)1 (char) 255 (char) 255 (char) 255 (char) 255 (char) 255 (char) 255 (char) 72

Che rappresenta:
  1. Codice *SEND_TAGS* : 203
  2. ID Antenna : 5
  3. Numero tags rilevati : 1
  4. MAC-1 : FF
  5. MAC-2 : FF
  6. MAC-3 : FF
  7. MAC-4 : FF
  8. MAC-5 : FF
  9. MAC-6 : FF
  10. Potenza RSSI: -72 db

Dal punto 4 al 9 è il codice MAC Address *FF:FF:FF:FF:FF:FF*