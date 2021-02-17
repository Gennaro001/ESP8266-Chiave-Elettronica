/*******************************************************************
 * Chiave Elettronica
 * by Gennaro Gaglione ge.gaglione@gmail.com
********************************************************************/

#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>


ESP8266WebServerSecure server(443);

/**************************************************************************************
 * il certificato e la chiave sono memorizzati in DRAM
***************************************************************************************/
      static const uint8_t x509[] PROGMEM = {
       #include "x509.h"
     };

      static const uint8_t rsakey[] PROGMEM = {
       #include "key.h"
     };

/**************************************************************************************
 * dichiarazione funzioni
***************************************************************************************/
String getContentType(String filename); // converte l'estensione nel relativo tipo MIME
bool handleFileRead(String path);       // invia il giusto file (se esiste)


/**************************************************************************************
 * definizione del pin a ci collegare il dispositivo di controllo
 * sostituire con il pin del dispositivo
**************************************************************************************/
#define PIN_RELE LED_BUILTIN

/**************************************************************************************
 * dopo l'installazione occorre definire una password di default
 * dopo il primo avvio la password viene registrata ne SPIFFS e può essere modificata
 * con l'opzione password nella schermata iniziale
**************************************************************************************/
String password="12345678";

void scriviPassword()
{
  File f = SPIFFS.open("/pass.dat", "w");
  f.print(password);
  f.close();
}

void leggiPassword()
{
  Serial.println("lettura password");
  File f = SPIFFS.open("/pass.dat", "r");
  
  password=f.readStringUntil('\n');
  f.close();
}

void setup() {

  Serial.begin(115200);         // inizializza la comunicazione seriale con il computer
  delay(10);
  Serial.println('\n');
  
  SPIFFS.begin();               // avvia SPI Flash Files System


/*******************************************************************************
 * se non esiste il file pass.dat registra la password
 *******************************************************************************/
  if(!SPIFFS.exists("/pass.dat"))scriviPassword();
  leggiPassword();

/*******************************************************************************
 * inizializzazione della porta del dispositivo
 * N.B. se il PIN_RELE è impostato sul led della scheda il led sarà acceso 
 * se la tensione al pin è bassa
 ******************************************************************************/
  pinMode(PIN_RELE, OUTPUT);  
  digitalWrite(PIN_RELE, LOW);


  
  
  WiFi.begin("My-SSID", "My-password");
  Serial.println("");
 
  Serial.println("connessione in corso ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // attesa della connessione
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // invia sulla seriale su quale rete siamo collegati
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // invia sulla seriale l'indirizzo IP

  if (MDNS.begin("chiave")) {              // Start the mDNS responder for  chiave.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  /********************************************************************************************
   * imposta i valori delle chiavi
   *******************************************************************************************/
  server.getServer().setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));

/**********************************************************************************************
 * Se il client richiede un URI, invialo se esiste altrimenti 
 * rispondi con un errore 404 (non trovato)
 **********************************************************************************************/
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();                           // avia il server
  Serial.println("HTTP server avviato");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}


String getContentType(String filename) { // converte l'estenzione file ner corrispettivo MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // invia il file richiesto (se esiste)
  String pwd1="",pwd2="";
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // se la richiesta è per la cartella radice invia index.html
  String contentType = getContentType(path);            // controlla il tipo MIME
  if (SPIFFS.exists(path)) {                            // se il file esiste
    if(path == "/index.html"){                          // se index.html resetta timers e porte
      if (server.args() > 0) {
        for (uint8_t i = 0; i < server.args(); i++) {
          Serial.println("argument name"+server.argName(i)+" "+server.arg(i));
          if (server.argName(i) == "pwd") {
            if(server.arg(i)==password){
              for (uint8_t j = 0; j < server.args(); j++) {
                if (server.argName(j) == "azione") {
                  Serial.println("azione= "+server.arg(j));
                  if(server.arg(j)=="attiva")digitalWrite(PIN_RELE, HIGH);
                  if(server.arg(j)=="disattiva")digitalWrite(PIN_RELE, LOW);
                  if(server.arg(j)=="password")path="/password.html";
                  else {
                    if(digitalRead(PIN_RELE)==HIGH) path="/attivata.html";
                    else  path="/disattivata.html";
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if(path == "/password.html"){ 
        //gestione della password
        if (server.args() > 0) {
        for (uint8_t i = 0; i < server.args(); i++) {
          if (server.argName(i) == "pwd") {
            if(server.arg(i)==password){
              for (uint8_t j = 0; j < server.args(); j++) {
                if (server.argName(j) == "pwd1") pwd1=server.arg(j);
                if (server.argName(j) == "pwd2") pwd2=server.arg(j);
                  
                }
              }
              if(pwd1.length()<8)path="/errpass2.html";
              else if(pwd1!=pwd2)path="/errpass1.html";
              else{
                password=pwd1;
                scriviPassword();
                path="/index.html";
              }
            }
          }
        }
      }

      
    Serial.println("send "+path);
    File file = SPIFFS.open(path, "r");                 // apri il file richiesto
    size_t sent = server.streamFile(file, contentType); // ed invialo al client
    file.close();                                       // quindi chiudi il file
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // se il file non esiste ritorna falso
}
