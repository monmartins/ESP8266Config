#include "web.h"



namespace web {

    // Server and other global objects
    ESP8266WebServer server(80);
    DNSServer dns;
    //
    const char* www_username = "admin";
    const char* www_password = "JUFesp82662022*";
    // allows you to set the realm of authentication Default:"Login Required"
    const char* www_realm = "Auth Required";
    // the Content of the HTML response in case of Unautherized Access Default:empty
    String authFailResponse = "Authentication Failed";
    
    void handleWifi() {
        String draw = server.arg("plain");
        DynamicJsonDocument doc(512);
        doc["name_wifi"] = WiFi.softAPSSID();
        doc["ip"] = WiFi.softAPIP();
        char json[512];
        serializeJson(doc, json);
        server.send(200, "application/json",json);
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
    bool auth(){
        if (!server.authenticate(www_username, www_password))
        // Basic Auth Method with Custom realm and Failure Response
        // return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        // Digest Auth Method with realm="Login Required" and empty Failure Response
        // return server.requestAuthentication(DIGEST_AUTH);
        // Digest Auth Method with Custom realm and empty Failure Response
        // return server.requestAuthentication(DIGEST_AUTH, www_realm);
        // Digest Auth Method with Custom realm and Failure Response
        {
            server.sendHeader("Location", String("/"), true);
            server.send (302, "text/plain", "Login Failed");
            server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
            return false;
        }
        return true;
    }
    void handleRoot() {
        if(auth()) return;
        server.sendHeader("Location", String("/index.html"), true);
        server.send (302, "text/plain", "Login Ok");
    }

    void handleNotFound(){
        // if(auth()) return;
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
        // if(auth()) return;
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