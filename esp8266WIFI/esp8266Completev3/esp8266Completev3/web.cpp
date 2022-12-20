#include "web.h"

namespace web {

    // Server and other global objects
    ESP8266WebServer server(80);
    DNSServer dns;
    
    void handleWifi() {
        String draw = server.arg("plain");
        DynamicJsonDocument doc(512);
        doc["name_wifi"] = WiFi.SSID();
        doc["ip"] = WiFi.softAPIP();
        char json[512];
        serializeJson(doc, json);
        server.send(200, "application/json",json);
        // server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
    }
    void handleGetInfoWifi() {
        String draw = server.arg("plain");
        DynamicJsonDocument doc(512);
        doc["name_wifi"] = WiFi.SSID();
        doc["ip"] = WiFi.softAPIP();
        char json[512];
        serializeJson(doc, json);
        server.send(200, "application/json",json);
    }
    void handleWifiList() {
        String draw = server.arg(0);
        DynamicJsonDocument doc(2048);
        doc["draw"]=draw;
        
        int numberOfNetworks = WiFi.scanNetworks();
        doc["recordsTotal"]=numberOfNetworks;
        doc["recordsFiltered"]=numberOfNetworks;
        DynamicJsonDocument docInput(200);
        for(int i =0; i<numberOfNetworks; i++){
            docInput["name_ssid"]=WiFi.SSID(i);
            docInput["signal_strength"]=WiFi.RSSI(i);
            docInput["access"]="/api/wifi/profile";
            doc["data"][i]=docInput;
        }
        char json[2048];
        serializeJson(doc, json);
        Serial.print("back: ");
        Serial.println(json);
        server.send(200, "application/json", json);
        
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

    void startWeb(){
        Serial.println('\n');
        Serial.print("Connected to ");
        Serial.println(WiFi.SSID());              // Tell us what network we're connected to
        Serial.print("IP address:\t");
        Serial.println(WiFi.softAPIP());           // Send the IP address of the ESP8266 to the computer

        if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
            Serial.println("mDNS responder started");
        } else {
            Serial.println("Error setting up MDNS responder!");
        }

        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
        //->    /api/wifi/
        server.on("/api/wifi/", HTTP_POST,handleWifi); 
        //->    /api/wifi/
        server.on("/api/wifi/list", HTTP_POST,handleWifiList); 
        server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

        server.begin();                           // Actually start the server
        Serial.println("HTTP server started");
    }

    void loopServer(){
      server.handleClient();                    // Listen for HTTP requests from clients
    }

    void startDNS(IPAddress local_IP){
        dns.setErrorReplyCode(DNSReplyCode::NoError);
        dns.start(53, "*", local_IP);
    }
        
    void loopDNS(){
        dns.processNextRequest();
    }
        
}