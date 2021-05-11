#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "speedy@a5e4";
const char* password = "kmzwa8awaa";

int led_merah = 23;
int led_kuning = 22;
int led_hijau = 3;

//Document size
DynamicJsonDocument doc(1024);

unsigned long curTime = millis();
unsigned long prevTime = 0;
const long timeoutTime = 1 * 60 * 1000; //1 menit sekali

void TimeTemp() {
  HTTPClient http;
  curTime = millis();
  
  if (curTime - prevTime > timeoutTime){
    Serial.println("Send request");
    prevTime = curTime;

    String request = "https://api.tomorrow.io/v4/timelines?location=-7.7860493,110.376187,17&timezone=Asia/Bangkok&fields=temperature&timesteps=current&units=metric&apikey=H8IGvDPdoVzHTQlP23GY3zcYRP6SdWf8";

    http.begin(request);
    http.GET();
  
    String response = http.getString();

    DeserializationError error = deserializeJson(doc, response);
    if(error){
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      
    }

    JsonObject data_timelines = doc["data"]["timelines"][0];
    const char* dt_intervals_startTime = data_timelines["intervals"][0]["startTime"];
    float dt_intervals_values_temperature = data_timelines["intervals"][0]["values"]["temperature"];

    Serial.print("Waktu: ");
    Serial.println(dt_intervals_startTime);
    Serial.print("Temperature Suhu : ");
    Serial.println(dt_intervals_values_temperature);

    if(dt_intervals_values_temperature > 30) {
      Serial.println("Level AC: Panas");
      Serial.println("Indikator: Merah");
      digitalWrite(led_merah, HIGH);
    }
    else if(dt_intervals_values_temperature <= 30 && dt_intervals_values_temperature >= 25) {
      Serial.println("Level AC: Sedang");
      Serial.println("Indikator: Kuning");
      digitalWrite(led_kuning, HIGH);
    }
    else if(dt_intervals_values_temperature < 25){
      Serial.println("Level AC: Dingin");
      Serial.println("Indikator: Hijau");
      digitalWrite(led_hijau, HIGH);
    }

    Serial.println("");
    
    //Close connection
    http.end();
  }
 
}

void setup() {
  Serial.begin(115200);

  //koneksi wifi  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  pinMode(led_merah, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_hijau, OUTPUT);
  
}

void loop() {
  TimeTemp();
  
}
