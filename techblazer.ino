#include <WiFi.h>
const char* ssid     = "surfACe";
const char* password = "surfACe";
WiFiServer server(80);
String header;

void setup(){


  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  }

void loop(){
  WiFiClient client = server.available(); 
  if (client){
    String currentLine = "";  
     while (client.connected()){
        if (client.available()){
            char c = client.read(); 
            header += c;
            if (c == '\n') {  
              if (currentLine.length() == 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();

                if (header.indexOf("GET /start") >= 0){
                    Serial.println("Start");

                    client.println("<!DOCTYPE html><html>");
                    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                    client.println("<link rel=\"icon\" href=\"data:,\">");
                    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                    client.println(".button2 {background-color: #555555;}</style></head>");
    
                    client.println("<body><h1>surfACe</h1>");
                    client.println("<p><a href=\"/stop\"><button class=\"button\">STOP</button></a></p>");
                    client.println("</body></html>");
                    client.println();

                    while (header.indexOf("GET /stop") <0){
                        //run program code
                      }
                    
                  }

                else if (header.indexOf("GET /debug") >=0){
                    Serial.println("Debug")
                    //debug
                  }
               else {

                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                client.println(".button2 {background-color: #555555;}</style></head>");

                client.println("<body><h1>surfACe</h1>");
                client.println("<p><a href=\"/start\"><button class=\"button\">START</button></a></p>");
                client.println("<p><a href=\"/debug\"><button class=\"button\">DEBUG</button></a></p>");
                client.println("</body></html>");
                client.println();
               }
                break;
              } else{
                  currentLine = "";
                } 
            } else if (c != '\r') {
                 currentLine += c;  
              }
           }
        }
        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");  
    }
  }
