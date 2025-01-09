# ArduinoNIC - Rete a Bus Simile a ISO/OSI

Benvenuti nel progetto **ArduinoNIC**, una rete a bus progettata per simulare il funzionamento dello stack ISO/OSI fino al livello 3. L’obiettivo è creare un sistema in cui Arduino gestisce l’incapsulamento dei dati attraverso più livelli, aggiungendo header specifici a ciascun livello, e funge da **Network Interface Controller (NIC)** per dispositivi esterni.

___

## 🧰 Caratteristiche del progetto  
- **Rete a bus** gestita da Arduino tramite seriale.  
- **Incapsulamento dei dati** in più strati, seguendo i principi ISO/OSI.  
- Supporto per una **libreria Python** che comunica con Arduino per inviare e ricevere pacchetti.  
- Struttura **modulare**: ogni livello aggiunge il proprio header e controllo.  
- Ideale per chi vuole **imparare o sperimentare** con i protocolli di rete su hardware a basso costo.

___
## La struttura del pacchetto

|     | Dimensione (bit) | Descrizione |
| --- | --- | --- |
| **Livello Fisico** |     |     |
| Preamble | 16  | Sequenza di sincronizzazione (0xAA). |
| **Livello Data Link** |     |     |
| Indirizzo Dest. | 16  | Indirizzo fisico sul bus. |
| Indirizzo Mitt. | 16  |  |
| Tipo Protocollo | 8   | Protocollo superiore. |
| Checksum | 16  | Verifica integrità del livello 2. |
| **Livello Rete** |     |     |
| Indirizzo IP Dest. | 32  | Indirizzo logico del destinatario. |
| Indirizzo IP Mitt. | 32  | Indirizzo logico del mittente. |
| TTL | 8   | Tempo di vita. |
| Checksum | 16  | Verifica integrità livello 3. |
| **Livello Trasporto** |     |     |
| Porta Dest. | 16  | Porta logica del destinatario. |
| Porta Mitt. | 16  | Porta logica del mittente. |
| Seq. Number | 32  | Numero di sequenza. |
| Ack. Number | 32  | Numero di ack. |
| Flags | 8   | Controllo connessione. |
| Window Size | 16  | Dimensione della finestra. |
| Checksum | 16  | Verifica integrità livello 4. |
| **Livello Applicativo** |     |     |
| Dati | 128   | Contenuto effettivo del messaggio. |
___

## 📝 Nota Importante  

Non sono un esperto di networking o sistemi embedded, quindi il codice potrebbe non essere il più ottimizzato o robusto. Questo progetto è stato sviluppato per divertimento e per imparare.  

Se hai suggerimenti, miglioramenti o vuoi contribuire, sei il benvenuto! 😊  

___

## 📫 Contatti  

Per domande o suggerimenti, sentiti libero di aprire una **issue** o contattarmi direttamente!  

Grazie per il tuo interesse nel progetto! 💡
