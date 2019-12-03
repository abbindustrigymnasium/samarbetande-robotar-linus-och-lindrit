#include "EspMQTTClient.h"
#define pwm 5
#define dir 0
#define hallGivarePin 13

float rev = 0, currentMillis = 0, startMillis = 0, diffMillis = 0, currentPulse = 0, startPulse = 0, diffPulse = 0, circumference = 11.6;
float error = 0, k = 0.5, isValue = 0, shouldValue = 50, enginePower = 800;

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
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(INPUT, hallGivarePin);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(hallGivarePin), HtoL, FALLING);
}

ICACHE_RAM_ATTR void HtoL() {
  rev++;
  //Serial.println(rev);
}

void onConnectionEstablished()
{
  client.subscribe("linus.kasper@abbindustrigymnasium.se/logger", [] (const String &payload)
  {
    //enginePower = payload.toInt();
  });
}

float dTime(){
  currentMillis = millis();
  diffMillis = currentMillis - startMillis;
  startMillis = currentMillis;
  return diffMillis;
}

float dPulse(){
  currentPulse = rev;
  diffPulse = currentPulse - startPulse;
  startPulse = currentPulse;
  return diffPulse * 1000;
}

float getVelocity(){ 
  return ((dPulse() / 96 * 1.2) * circumference) / dTime();
}

void loop() {
  client.loop();
  digitalWrite(dir, HIGH);
  analogWrite(pwm, enginePower);
  isValue = getVelocity();
  error = shouldValue - isValue;
  enginePower += k * error;
  client.publish("linus.kasper@abbindustrigymnasium.se/logger", "ärvärde" + String(isValue) + " börvärde" + String(shouldValue));
  //Serial.print("Ärvärde: " + String(isValue));
  //Serial.print(" Börvärde: " + String(shouldValue));
  //Serial.print(" Fel: " + String(error));
  //Serial.println(" pwm: " + String(enginePower));
  delay(1000);
}