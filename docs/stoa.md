# Comunicazione Server > Antenna

Questo tipo di comunicazione si basa su semplici comandi inviati dal server all'antenna tramite protocollo UDP/IP.

Il server deve comunicare all'antenna sulla porta stabilita dall'antenna. Per sapere le porte di tutte le antenne basta inviare un messggio Broadcast con in codice *REQUEST_SYNC*.

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
    <td>220</td>
    <td>REQUEST_SYNC</td>
  </tr>
  <tr>
    <td>221</td>
    <td>REQUEST_TAGS</td>
  </tr>
  <tr>
    <td>254</td>
    <td>REQUEST_DEEP_SLEEP</td>
  </tr>
  <tr>
    <td>255</td>
    <td>REQUEST_STOP</td>
  </tr>
</tbody>
</table>

> ## Per sapere come risponde un'antenna vedere [qui](atos.md)

#### 1. **REQUEST_SYNC**
Richiede all'antenna di mandare le informazioni iniziali. L'antenna risponde con il codice *SEND_INFO*. 
Quest'informazione è mandata ad inizio comunicazione senza che sia necessario mandare la richiesta.

#### 2. **REQUEST_TAGS**
Chiede all'antenna di eseguire la scansione. A fine processo l'antenna comunica i risultati con il codice *SEND_TAGS*.
L'antenna impiega un tempo non prestabilito per eseguire la scansione poichè dipende dal numero di reti presenti e rilevabili in quel momento.

#### 3. **REQUEST_DEEP_SLEEP** --*Non ancora implementato*
Manda un'antenna in modalità DEEP_SLEEP, è necessario inviare anche un tempo di DEEP_SLEEP dopo il quale l'antenna si risveglia per tot tempo e se non riceve una nuova comunicazione ritorna in DEEP_SLEEP per lo stesso tempo.

#### 4. **REQUEST_STOP** --*Non ancora implementato*
Comunica all'antenna di fermare il client. Dopo tot. secondi specificati dal server come argomento di questo comando, il client riparte.
