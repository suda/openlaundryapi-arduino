/*

 This is OpenLaundryAPI client.
 
 Connects to wi-fi network and sends 
 data received from analog input
 
 created 10 November 2012
 by mkozak (github.com/meal) - OpenLaundryAPI team (github.com/OpenLaundryAPI)
 
 */
#include <WiFi.h>
#include <SPI.h>

WiFiClient client;

char ssid[] = "Video"; // your network SSID
char pass[] = "blaipvideo"; // your network password
char device[] = "stara"; // your device ID
char token[] = "testmatkozak"; // secret auth token
int status = WL_IDLE_STATUS;
const int analogInPin = A0;
const int analogOutPin = 9;
char serverName[] = "openlaundryapi.org";
int sensorValue = 0;
int outputValue = 0;

String data = "";

void setup() {

  data.reserve(600);
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
}

void loop() {
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  collectData(); 
}

String collectData() {
  int i;  
  for(i=0; i<=600; i++ ) {
    sensorValue = analogRead(analogInPin);
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    analogWrite(analogOutPin, outputValue);
    data += sensorValue;
    data += ",";
    Serial.println(sensorValue);
  }
 
  sendData(data);
  i=0;
}

void sendData(String data) {
  if (client.connect(serverName, 80)) {
    Serial.print("connected to server\n");
    client.println("POST /api/device/stara/testmatkozak/ HTTP/1.1");
    Serial.print("POST\n");
    client.println("Host:openlaundryapi.org");
    Serial.print("HOST\n");
    client.print("Content-Length: ");
    client.println(data.length());
    Serial.println(data.length());
    Serial.print("data length\n");
    client.println();
    client.print(data);
    Serial.print("data:\n");
    Serial.print(data);
    client.println();
  } 
}



