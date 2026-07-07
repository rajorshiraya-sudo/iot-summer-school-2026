/*
Project: MQTT Sensor Publisher
Author: SATYAM KUMAR
Desc: esp32 publishes temperature and humidity to an mqtt broker and can toggle an led from a topic
Date: 2026-07-04
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

int dhtPin=15;
int led=2;

DHT dht(dhtPin,DHT22);
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic,byte* message,unsigned int length){
String msg="";
for(int i=0;i<length;i++){msg=msg+(char)message[i];}
if(msg=="on"){digitalWrite(led,HIGH);}
if(msg=="off"){digitalWrite(led,LOW);}}

void setup(){
Serial.begin(115200);
dht.begin();
pinMode(led,OUTPUT);

WiFi.begin("Wokwi-GUEST","");
while(WiFi.status()!=WL_CONNECTED){delay(500);Serial.print(".");}
Serial.println("WiFi connected");

client.setServer("broker.hivemq.com",1883);
client.setCallback(callback);}

void reconnect(){
while(!client.connected()){
Serial.println("Connecting to MQTT...");
if(client.connect("esp32-satyam-123")){
Serial.println("MQTT connected");
client.subscribe("iitjammu/summer26/satyam/led_control");}
else{delay(2000);}}
}

void loop(){
if(!client.connected()){reconnect();}
client.loop();

float temp=dht.readTemperature();
float hum=dht.readHumidity();

String tempMsg="{\"value\": "+String(temp)+", \"unit\": \"C\"}"; 
String humMsg="{\"value\": "+String(hum)+", \"unit\": \"%\"}"; 

client.publish("iitjammu/summer26/satyam/temperature",tempMsg.c_str());
client.publish("iitjammu/summer26/satyam/humidity",humMsg.c_str());

Serial.print("Published -> ");
Serial.print(tempMsg);
Serial.print(" | ");
Serial.println(humMsg);

delay(5000);}
