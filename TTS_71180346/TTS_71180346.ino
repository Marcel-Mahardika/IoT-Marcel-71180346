#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = "hki9yDcHbOhSOdPzHX0v1rLKXY1ylshA";
char ssid[] = "speedy@a5e4";
char pass[] = "kmzwa8awaa";

#define DHTPIN 23   //sensor suhu
#define DHTTYPE DHT11   //tipe DHT
DHT dht(DHTPIN, DHTTYPE);  
BlynkTimer timer;   

//indikator
WidgetLED ledLight(V6);
WidgetLED ledTemp(V7);

int slider_light;
int slider_temp;
int button;
int led_light = 22;
int led_temperature = 3;
float t;  
int lux;

BLYNK_CONNECTED(){
  Blynk.syncAll();
}

//slider cahaya
BLYNK_WRITE(V4){
  slider_light = param.asInt();
}

//slider suhu
BLYNK_WRITE(V5) {
  slider_temp = param.asInt();
}

//switch button
BLYNK_WRITE(V8) {
  button = param.asInt();
}

//fungsi sensor suhu
void temperatureSensor() {
  if(button == 1) {
    t = dht.readTemperature();
    Serial.println(t);
    if(isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    Blynk.virtualWrite(V1, t);
    Blynk.virtualWrite(V3, t);
  }
  else {
    Serial.println("Sistem Berhenti");
  }
     
}

//fungsi sensor cahaya
void lightSensor() {
  int lightSensor = 34;
  if(button == 1) {
    lux = (0.009768*analogRead(lightSensor))+10;
    Serial.println(lux);
    Blynk.virtualWrite(V0, lux);
    Blynk.virtualWrite(V2, lux);
  }
  else {
    Serial.println("Sistem Berhenti");
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(72,44,45,151), 8080);
 
  dht.begin();
  timer.setInterval(500L, temperatureSensor);
  timer.setInterval(500L, lightSensor);

  pinMode(led_light, OUTPUT);
  pinMode(led_temperature, OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();

  if(button == 1) {
    //slider cahaya
    if(lux >= slider_light) {
      digitalWrite(led_light,HIGH);
      ledLight.on();
    }
    else{
      digitalWrite(led_light,LOW);
      ledLight.off();
    }
   
    //slider suhu
    if(t >= slider_temp){
      digitalWrite(led_temperature,HIGH);
      ledTemp.on();
    }
    else{
      digitalWrite(led_temperature,LOW);
      ledTemp.off();
    }
  }
  else {
    Serial.println("Sistem Berhenti");
  }
  
}
