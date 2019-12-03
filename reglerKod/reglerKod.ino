#include "EspMQTTClient.h"
#define pwm 5
#define dir 0
#define hallGivarePin 13

int rev = 0, velocity = 0;
bool countRotation = false;

void onConnectionEstablished();

EspMQTTClient client(
 "ABB_Indgym_Guest",
 "Welcome2abb",
 "maqiatto.com",
 1883,
 "linus.kasper@abbindustrigymnasium.se",
 "1234",
 "logger",
 onConnectionEstablished, // Connection established callback
 true,
 true    
);

void setup() {
  Serial.begin(115200);
  pinMode(hallGivarePin, INPUT);
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(hallGivarePin), addRev, FALLING);
}

void onConnectionEstablished()
{
  client.subscribe("linus.kasper@abbindustrigymnasium.se/logger", [] (const String &payload)
  {
    velocity = payload.toInt();
  });
}

ICACHE_RAM_ATTR void addRev(){
  if (countRotation){
    rev++;
  }
}

int millisPerRotation(){
  countRotation = true;
  int startMillis = millis();
  while (true){
    if (rev > 127){ //127 pulser är ett varv
      countRotation = false;
      rev = 0;
      int currentMillis = millis();
      int diffMillis = currentMillis - startMillis;
      Serial.println(diffMillis);
      return 12 / (diffMillis * 3600000);
    }   
  }
}

void loop() {
  client.loop();
  digitalWrite(dir, HIGH);
  analogWrite(pwm, velocity);
  Serial.println("Rotation (cm/min): " + String(millisPerRotation()));
}
