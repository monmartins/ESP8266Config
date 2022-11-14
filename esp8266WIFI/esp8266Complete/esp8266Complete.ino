#include "ESP8266WiFi.h"
//
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <FS.h>   // Include the SPIFFS library
 
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
//FS
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
//http
void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();
void setup() {
    Serial.begin(115200);

    listWifi();
    //

    const char *ssid = "ESP8266-Test2"; // The name of the Wi-Fi network that will be created
    const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open  
    startAP(ssid,password);
    SPIFFS.begin();                           // Start the SPI Flash Files System
    startWebServer();
 
}
void startWebServer(){
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

    if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    } else {
    Serial.println("Error setting up MDNS responder!");
    }

    server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
    server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin();                           // Actually start the server
    Serial.println("HTTP server started");
}
void startMultiClientAP(const char *ssid, const char *password){
    wifiMulti.addAP(ssid, password);
    while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        delay(250);
        Serial.print('.');
    }

}
void startAP(const char *ssid, const char *password){
    Serial.begin(115200);
    delay(10);
    Serial.println('\n');

    WiFi.softAP(ssid, password);             // Start the access point
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");

    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
}
void listWifi(){
  int numberOfNetworks = WiFi.scanNetworks();
 
  for(int i =0; i<numberOfNetworks; i++){
 
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
 
  }
}
 
// void loop() {}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  if (!handleFileRead(server.uri()))
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){  // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.html";           // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){  // If the file exists, either as a compressed archive, or normal
    if(SPIFFS.exists(pathWithGz))                          // If there's a compressed version available
      path += ".gz";                                         // Use the compressed version
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;                                          // If the file doesn't exist, return false
}
