#include <WiFi.h>
#include <MQTT.h>

//RELAY
#define RELAY 22

const char ssid[] = "HotspotGG";
const char pass[] = "kmzwa8awaa";

WiFiClient net;
MQTTClient client;

char on_off[10];
char tampil_cahaya[10];
int timer;
int ldr = 34;
int lux;
int led = 3;

//untuk koneksi
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("node_1", "", "")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  //subscribe ke topic yang ada di MQTT
  client.subscribe("node_1/on_off");
  client.subscribe("node_1/cahaya");
}

//untuk nerima data dari MQTT
void messageReceived(String &topic, String &payload) {
  if(topic == "node_1/on_off") {
    strcpy(on_off, payload.c_str());
  }
  if(topic == "node_1/cahaya") {
    strcpy(tampil_cahaya, payload.c_str());
  }
}

//untuk fitur pencahayaan
void Lighting() {
  lux = (0.009768*analogRead(ldr))+10;
  Serial.println(lux);
  client.publish("node_1/cahaya", itoa(lux, tampil_cahaya, 10));
  delay(1000);
  //untuk sensor cahaya
  if(lux <= 12) {
      digitalWrite(led,HIGH);
  }
  else{
    digitalWrite(led,LOW);
  }
}


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  client.begin("54.156.88.229", net);  //connect ke AWS EC2
  client.onMessage(messageReceived);  //jalankan fungsi message
  connect();
  
  pinMode(RELAY, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  client.loop();
  delay(10);
  if (!client.connected()) {
    connect();
  }

  Lighting();

  if(atoi(on_off) == 1) {
    digitalWrite(RELAY, LOW);
    Serial.println("Nyala");
  }
  else {
    digitalWrite(RELAY, HIGH);
    Serial.println("Mati"); 
  }
  
}
