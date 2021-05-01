#include <DHT.h>
#include <WiFi.h>
#include <MQTT.h>

//DHT 11
#define DHTPIN 23
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);  

//RELAY
#define RELAY 22

const char ssid[] = "speedy@a5e4";
const char pass[] = "kmzwa8awaa";

WiFiClient net;
MQTTClient client;

char tampil_suhu[10];
char pembatas[10];
char on_off[10];

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("AinzBoard", "ainzcient", "jrJPbW6mTT4qYGGt")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  //subscribe ke topic yang di MQTT
  client.subscribe("/miniproject2/pembatas");
  client.subscribe("/miniproject2/on_off");
}

void messageReceived(String &topic, String &payload) {
  if(topic == "/miniproject2/pembatas") {
    strcpy(pembatas, payload.c_str());
  }
  if(topic == "/miniproject2/on_off") {
    strcpy(on_off, payload.c_str());
  }
}

int temperature;

void TempSensor() {
  temperature = dht.readTemperature();
  Serial.print("Temperature : ");
  Serial.println(temperature);
  client.publish("/miniproject2/suhu", itoa(temperature, tampil_suhu, 10));
  delay(1000);

  if(temperature > atoi(pembatas)) {
    digitalWrite(RELAY, LOW);
  }
  else if(temperature <= atoi(pembatas)) {
    digitalWrite(RELAY, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  WiFi.begin(ssid, pass);
  client.begin("public.cloud.shiftr.io", net); //connect ke server shifter
  client.onMessage(messageReceived);  //jalankan fungsi message
  connect();
  
  dht.begin();
  pinMode(RELAY, OUTPUT);

}

void loop() {
  client.loop();
  delay(10);
  if (!client.connected()) {
    connect();
  }

  if(atoi(on_off) == 1) {
    TempSensor();
  }
  else {
    digitalWrite(RELAY, HIGH);
  }
  
}
