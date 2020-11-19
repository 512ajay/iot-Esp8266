#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include "DHT.h"
#include <DHT_U.h>

#define LED (2)
#define DHTPIN 12 
#define DHTTYPE DHT11
const char* ssid = "your wifi name";
const char* password = "wifi password";

const char* mqtt_server = "server name or ip address";

const char* mqtt_username = "server username";
const char* mqtt_password = "server password";
String clientID = "esp8266";
const char* topic = "topic/relay1";
const char* topic1 = "topic/mess";
const char* topic2 = "topic/temp";
const char* topic3 = "topic/publish";

DHT_Unified dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  pinMode(LED, OUTPUT);      
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  connectWifi();  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
    if (!client.connected()) {
      connectMQTT();
    }
    readTemp();
    client.loop();
    delay(500);
}
void connectWifi() {
  delay(10);
  Serial.printf("\nConnecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected on IP address ");
  Serial.println(WiFi.localIP());
}
void connectMQTT() {
  while (!client.connected()) {
    // Create a random client ID
    clientID += String(random(0xffff), HEX);
    Serial.printf("MQTT connecting as client %s...\n", clientID.c_str());
 
    if (client.connect("1234", mqtt_username, mqtt_password)) {
      Serial.println("MQTT connected");
      client.subscribe(topic);
      client.subscribe(topic1);
    } else {
      Serial.printf("MQTT failed, state %s, retrying...\n", client.state());
      delay(2500);
    }
  }
}
void callback(const char* topic, byte* payload, unsigned int length){
 String messageTemp;
 for (int i = 0; i < length; i++) {
  messageTemp += (char)payload[i];
 }
 if(strcmp(topic, "topic/relay1") ==0){
  
   if(messageTemp =="on"){
    setLedState(HIGH);
   }

   if(messageTemp =="off"){
    setLedState(LOW);
   }
   
 Serial.println("Publish received.");
 Serial.print(" message: ");
 Serial.println(messageTemp);
 Serial.print(" topic: ");
 Serial.println(topic);
 }
 if(strcmp(topic1, "topic/mess") ==0){
 Serial.println("Publish received.");
 Serial.print(" message: ");
 Serial.println(messageTemp);
 Serial.print(" topic: ");
 Serial.println(topic1);
 }

}
void setLedState(boolean state) {
  digitalWrite(LED, !state);
}
void readTemp(){
   sensors_event_t event;
  dht.temperature().getEvent(&event);
      client.publish(topic3, String(event.temperature).c_str(), true);
      client.publish(topic2, "on");
      

}
