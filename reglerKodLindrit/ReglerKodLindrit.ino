#include "EspMQTTClient.h"
#define pwm 5
#define dir 0
#define hallGivarePin 13

float rev, currentMillis, startMillis, diffMillis, currentPulse, startPulse, diffPulse, error, timeError, isValue, currentTime, diffTime, previousTime;
float kP = 0.001, kI = 10000, circumference = 3.7 * PI, setValue = 0, enginePower = 0;
bool drive = false;

void onConnectionEstablished();

EspMQTTClient client(
 "ABB_Indgym_Guest",
 "Welcome2abb",
 "maqiatto.com",
 1883,
 "linus.kasper@abbindustrigymnasium.se",
 "1234",
 "lindrit",
 onConnectionEstablished,
 true,
 true    
);

void setup() {
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(INPUT, hallGivarePin);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(hallGivarePin), addRev, FALLING);
}

void onConnectionEstablished()
{
  client.subscribe("linus.kasper@abbindustrigymnasium.se/logger", [] (const String &payload){
    if (payload.endsWith("/")){
      
    }else{
      setValue = payload.toInt();
      if (setValue > 0){
        drive = true;
        enginePower = 400;
      }
    }
  });
}

ICACHE_RAM_ATTR void addRev() {
  rev++;
}

float dTime(){
  currentMillis = millis();
  diffMillis = currentMillis - startMillis;
  startMillis = currentMillis;
  return diffMillis / 1000;
}

float dPulse(){
  currentPulse = rev;
  diffPulse = currentPulse - startPulse;
  startPulse = currentPulse;
  return diffPulse;
}

float getVelocity(){ 
  return ((dPulse() / 100) * circumference) / dTime();
}

void loop() {
  client.loop();
  digitalWrite(dir, HIGH);
  analogWrite(pwm, enginePower);
  if (drive == true){
    isValue = getVelocity();
    error = setValue - isValue;
    timeError += error * dTime();
    enginePower = (kP * error) + (kI * timeError);
    client.publish("linus.kasper@abbindustrigymnasium.se/logger","lindrit " + String(isValue) + " " + String(enginePower) + " /");
    if (enginePower > 1023){
      enginePower = 1023;
    }
  }
}
