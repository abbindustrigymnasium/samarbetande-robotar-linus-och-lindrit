#include "EspMQTTClient.h"
#define pwm_A 5
#define dir_A 0
#define hallGivare 13

int velocity=0;

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
}

void onConnectionEstablished()
{
  client.subscribe("linus.kasper@abbindustrigymnasium.se/logger", [] (const String &payload)
  {
    velocity = payload.toInt();
  });
}

void loop() {
  client.loop();
    digitalWrite(dir_A, HIGH);
    analogWrite(pwm_A, velocity);
  
}
