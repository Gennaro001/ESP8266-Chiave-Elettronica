# Esp8266 Chiave elettronica

## Introduzione

Questo sketch permete di gestire una chiave elettronica in sicurezza tramite l'HyperText Transfer Protocol over Secure Socket Layer e password di 8 o più caratteri.

Poiché è abbastanza fastidioso dover ricordare l'indirizzo IP della rete locale a cui è associata la chiave si usa il multicast DNS per associarlo all'indirizzo https://chiave.local

La password viene memorizzata nel SPIFFS dell'esp8266. In tal modo anche disalimentando il dispositivo non si resetta la password che al primo avvio è impostata su: 12345678.

## Modifiche da apportare allo sketch

PIN_RELE è la porta di attivazione del dispositivo collegato. Nello sketch è definito alla porta del led della scheda. Modificare la riga 29 per definire la porta a cui collegare il dispositivo.

```cpp
#define PIN_RELE LED_BUILTIN
```

> ricordate che il livello basso acccende il led ed il livello alto lo spegne l

editare la riga inserendo SSID e password della propria rete

```cpp
WiFi.begin("My-SSID", "My-password");
```

il multicast DNS può essere personalizzato sostituendo la parola "chiave" con quella desiderata nella riga:

```cpp
  if (MDNS.begin("chiave")) {              // Start the mDNS responder for  chiave.local
```

## Generazione del certificato

per poter utilizzare il protocollo SSL occorre generare un certificato. A tale scopo editare il file make-self-signed-cert.sh e modificare la riga 34 con una stringa di propria scelta

```
O = your-name-here
```

eseguendo il batch saranno generati due file x50.h e key.h che vengono inclusi automaticamente nello sketch.

## Installazionedello sketch

prima di caricare lo sketch occorre caricare sull'8266 la cartella dei dati dove sono archiviate le pagine web che saranno di volta in volta visualizzate.

Per fare ciò nell'Ide selezionare: Strumenti -> esp8266 Sketch Data Upload e, successivamente, caricare lo sketch.

> Qualora negli strumenti non trovate la voce suindicata dovrete installare il tools relativo in arduino al link [Using ESP8266 SPIFFS : 7 Steps - Instructables](https://www.instructables.com/Using-ESP8266-SPIFFS/) troverete le istruzioni per caricarlo

## Prima connessione dal pc

Collegandosi per la prima volta a https://chiave.local il sistema vi segnalerà che la chiave non è sicura non essendo il certificato emesso da un'autorità di certificazione. Procedete comunque abilitando e definendola come chiave sicura per il browser.

## Schermate della procedura

La schermata iniziale è raffigurata in figura. Inserita la pasword si può procedere all'attivazione, disattivazione o cambio password

![](file:///home/gennaro/.var/app/com.github.marktext.marktext/config/marktext/images/2021-02-15-16-47-57-image.png)

attivando o disattivando il dispositivo verrà visualizzata una pagina di conferma, cliccando su ok si ritorna alla pagina iniziale.

Poichè al primo avvio sull'8266 non ci sarà alcuna password registrata nell'archivio la password sarà settata a: **12345678**. cambiare la password ricordando che la stessa deve essere almeno di 8 caratteri.

## Licenza

Copyright (C) 2021 by Gennaro Gaglione 

Licenza MIT

Con la presente si concede, a chiunque ottenga una copia di questo software e dei file di documentazione associati (il "Software"), l'autorizzazione a usare gratuitamente il Software senza alcuna limitazione, compresi i diritti di usare, copiare, modificare, unire, pubblicare, distribuire, cedere in sottolicenza e/o vendere copie del Software, nonché di permettere ai soggetti cui il Software è fornito di fare altrettanto, alle seguenti condizioni:

L'avviso di copyright indicato sopra e questo avviso di autorizzazione devono essere inclusi in ogni copia o parte sostanziale del Software.

IL SOFTWARE VIENE FORNITO "COSÌ COM'È", SENZA GARANZIE DI ALCUN TIPO, ESPLICITE O IMPLICITE, IVI INCLUSE, IN VIA ESEMPLIFICATIVA, LE GARANZIE DI COMMERCIABILITÀ, IDONEITÀ A UN FINE PARTICOLARE E NON VIOLAZIONE DEI DIRITTI ALTRUI. IN NESSUN CASO GLI AUTORI O I TITOLARI DEL COPYRIGHT SARANNO RESPONSABILI PER QUALSIASI RECLAMO, DANNO O ALTRO TIPO DI RESPONSABILITÀ, A SEGUITO DI AZIONE CONTRATTUALE, ILLECITO O ALTRO, DERIVANTE DA O IN CONNESSIONE AL SOFTWARE, AL SUO UTILIZZO O AD ALTRE OPERAZIONI CON LO STESSO.

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
