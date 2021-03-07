//71180346

int led_merah = 23;
int led_kuning = 22;
int led_hijau = 1;
int status_led = LOW;

int count = 9;
unsigned long currentMillis;

unsigned long previousMillis = 0;
const long interval = 1000; 

void setup() {
  // put your setup code here, to run once:
  pinMode(led_merah, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_hijau, OUTPUT);
}

void gantiState() {
  if(status_led == LOW) {
    status_led = HIGH;
  } 
  else {
    status_led = LOW;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      //lampu merah
      if(count == 9 || count == 5 || count == 1){
        if(count != 1) {
          gantiState();   
        }  
        digitalWrite(led_merah, status_led);
      }

      //lampu kuning
      if(count == 5 || count == 4){ 
        gantiState();
        digitalWrite(led_kuning, status_led);
      }

      //lampu hijau
      if(count == 4 || count == 1){ 
        gantiState();
        digitalWrite(led_hijau, status_led);
      }
      
      if(count != 1){
        count--;
      }
      else {
        count = 9;
      }
    }
  
}
