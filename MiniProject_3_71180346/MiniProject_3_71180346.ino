#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "speedy@a5e4"
#define WIFI_PASSWORD "kmzwa8awaa"

#define FIREBASE_HOST "https://kipasangin-iot-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "qk3aHbaQF0WdCruNS4LpZncK2bpJSYo4b1LCqOIh"

//DHT 11
#define DHTPIN 23
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);  

//RELAY
#define RELAY 22

//Define FirebaseESP32 data object
FirebaseData data_object;

void Koneksi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(data_object, 1000 * 60);
  Firebase.setwriteSizeLimit(data_object, "tiny");
}

int temperature;
int pembatas;
int on_off;

void TempSensor() {
  temperature = dht.readTemperature();
  Serial.print("Temperature : ");
  Serial.println(temperature);
  Firebase.setInt(data_object, "/miniproject3/suhu", temperature);  //set data ke firebase
  delay(1000);
  Firebase.getInt(data_object, "/miniproject3/pembatas"); //ambil container pembatas
  pembatas = data_object.intData(); //ambil nilai data
  
  if(temperature > pembatas) {
    digitalWrite(RELAY, LOW);
  }
  else if(temperature <= pembatas) {
    digitalWrite(RELAY, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Koneksi();
  
  dht.begin();
  pinMode(RELAY, OUTPUT);

}

void loop() {
  Firebase.getInt(data_object, "/miniproject3/on_off"); //ambil container on_off
  on_off = data_object.intData(); //ambil nilai data
  
  if(on_off == 1) {
    TempSensor();
  }
  else {
    digitalWrite(RELAY, HIGH);
  }

  Serial.println(pembatas);
  
}
