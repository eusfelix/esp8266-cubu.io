#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22


const char* ssid     = "xxxxxx";
const char* password = "xxxxxx";
const char* mqttServer = "mqtt.cubu.io";
const char* deviceKey   = "xxxxxx";
float temp_c, humidity, oldtemp_c, oldhumidity = 0;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceKey, deviceKey, deviceKey)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
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
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}



void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(2000); //DHT22 is a very slooow sensor:(
  temp_c = dht.readTemperature();
  humidity = dht.readHumidity();

  if (abs(temp_c - oldtemp_c) > 0.2) {
    char activeTopic[200];
    char charBuf[20];
    //temp_c.toCharArray(charBuf, 20);
    dtostrf(temp_c, 2, 2, charBuf);
    strcpy(activeTopic, "/d/");
    strcat(activeTopic, deviceKey);
    strcat(activeTopic, "/o/temperature");
    Serial.print("Temperature sent: ");
    Serial.println(charBuf);
    client.publish(activeTopic, charBuf);
    oldtemp_c = temp_c;
  }
  if (abs(humidity - oldhumidity) > 0.2) {
    char activeTopic[200];
    char charBuf[20];
    //temp_c.toCharArray(charBuf, 20);
    dtostrf(humidity, 2, 2, charBuf);
    strcpy(activeTopic, "/d/");
    strcat(activeTopic, deviceKey);
    strcat(activeTopic, "/o/humidity");
    Serial.print("Humidity Sent: ");
    Serial.println(charBuf);
    client.publish(activeTopic, charBuf);
    oldhumidity = humidity;
  }
  
}
