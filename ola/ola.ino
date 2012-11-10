/*

This is OpenLaundryAPI client.

Connects to wi-fi network and sends 
json data received from analog input

created 10 November 2012
by mkozak (github.com/meal) - OpenLaundryAPI team (github.com/OpenLaundryAPI)

*/
#include <WiFi.h>
#include <SPI.h>

WiFiClient client;

char ssid[] = "BL_GUEST"; // your network SSID
char pass[] = "blaipblaip"; // your network password
int status = WL_IDLE_STATUS;
const int analogInPin = A0;
const int analogOutPin = 9;
char serverName[] = "openlaundryapi.org";
byte mac[6];
int sensorValue = 0;
int outputValue = 0;

const unsigned long requestInterval = 30*1000;    // delay between requests; 10 seconds

boolean requested;                     // whether request wass made  since connecting
unsigned long lastAttemptTime = 0;     // last time of connection to the server, in milliseconds

String data = "";

void setup() {
  data.reserve(128024);
  Serial.begin(9600);
  while(!Serial) {
    ; // Neede for Leonardo board only
  }
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFI shield not present");
    while(true);
  }
  
  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    
    status = WiFi.begin(ssid, pass);
    
    delay(1000); // wait 10 seconds for reconnection
  }
  
  Serial.print("You're connected to the network\n");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  connectToServer(); 
}

void loop() {
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
   
    
  } else if (millis() - lastAttemptTime > requestInterval) {
  
    connectToServer();
  }
}

void connectToServer() {
  
  if (client.connect(serverName, 80)) {
    Serial.println("connected to server");
    client.println("GET /api/device/666/ HTTP/1.1");
    client.println("Host:openlaundryapi.org");
    client.println("Keep-Alive: 115");
    client.println("Connection: keep-alive");
    client.println();
   }
}
