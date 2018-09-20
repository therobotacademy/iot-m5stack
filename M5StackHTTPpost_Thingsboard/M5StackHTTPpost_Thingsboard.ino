// https://techtutorialsx.com/2017/05/20/esp32-http-post-requests/
// It uses library 'ArduinoHttpClient' by Arduino v0.3.1

// POST request to URL Thingsboard IoT server

#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "Alestis_invitados";
const char* password =  "L2wifi2L";
 
void setup() {
 
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
   
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient http;   
    http.begin("http://ec2-34-245-115-53.eu-west-1.compute.amazonaws.com/api/v1/DHT_FROM_M5STACK/attributes");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
 
    int httpResponseCode = http.POST("{\"temperatura\": 26.99,\"humedad\" : 60.5}");   //Send the actual POST request
 
    if(httpResponseCode>0)  {
      String response = http.getString();                       //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
    }
    else  {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
  }
 else
    Serial.println("Error in WiFi connection");   
 
 delay(10000);  //Send a request every 10 seconds
}
