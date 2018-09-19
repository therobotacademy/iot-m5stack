// https://developers.wia.io/docs/espressif-esp32

#include <ArduinoJson.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <Arduino.h>


const char* ssid     = ""; // Your WiFi ssid
const char* password = ""; //Your Wifi password

WiFiClient client;
int status = WL_IDLE_STATUS;

// get this from the wia dashboard. it should start with `d_sk`
const char* device_secret_key = "";

// Wia API parameters
char server[] = "api.wia.io";
char path[] = "/v1/events";
int port = 80;

StaticJsonDocument<200> jsonBuffer;
HttpClient httpClient = HttpClient(client, server, port);
JsonObject root = jsonBuffer.to<JsonObject>(); 


void setup() {

  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = "temperature";
  root["data"] = 21.5;
Serial.println("connecting...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");

    postToWia(root);

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected()) {
    Serial.println();
    client.stop();

    delay(10000);
    //Do nothing forever
    for (;;)
      ;
  }
}

void postToWia(JsonObject& data) {
  String dataStr = ""; 
  serializeJson(data, dataStr);
  httpClient.beginRequest();
  httpClient.post(path);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", dataStr.length());
  httpClient.sendHeader("Authorization", "Bearer " + String(device_secret_key));
  httpClient.beginBody();
  httpClient.print(dataStr);  
  httpClient.endRequest();

}
