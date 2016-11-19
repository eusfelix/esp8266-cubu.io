
#include <ESP8266WiFi.h>

const char* ssid     = "FelixGuest";
const char* password = "internet";

const char* host = "mqtt.cubu.io";
const char* deviceKey   = "0bcdd1900431";


void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void loop() {
  // put your main code here, to run repeatedly:

}
