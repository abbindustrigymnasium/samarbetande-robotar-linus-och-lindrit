#include "EspMQTTClient.h"

//Setting output pins
#define pwm 5
#define dir 0
#define hallGivarePin 13

//Defining variables
float rev, currentMillis, startMillis, diffMillis, currentPulse, startPulse, diffPulse, error, timeError, isValue, currentTime, diffTime, previousTime;
float kP = 0.001, kI = 10000, circumference = 3.7 * PI, setValue = 0, enginePower = 0;
bool drive = false;

//MQTT setup
void onConnectionEstablished();

//Parameters needed for MQTT setup
EspMQTTClient client(
 "ABB_Indgym_Guest",
 "Welcome2abb",
 "maqiatto.com",
 1883,
 "linus.kasper@abbindustrigymnasium.se",
 "1234",
 "linus",
 onConnectionEstablished,
 true,
 true    
);

void setup() {
  //Defining inputs and outputs
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(INPUT, hallGivarePin);

  //Defining serial tranfer
  Serial.begin(9600);

  //Interupt function
  attachInterrupt(digitalPinToInterrupt(hallGivarePin), addRev, FALLING);
}

void onConnectionEstablished() {
  //Subscribing to MQTT topic
  client.subscribe("linus.kasper@abbindustrigymnasium.se/logger", [] (const String &payload){ 
    if (payload.endsWith("/")){
      //Do nothing
    }else{
      //setValue is equal to paylod
      setValue = payload.toInt();
      if (setValue > 0){
        //Only if setValue is greater than 0
        drive = true;
        enginePower = 400;
      }
    }
  });
}

//Function is called when the rev graph is interupted
ICACHE_RAM_ATTR void addRev() {
  //Add 1 to the rev variable
  rev++;
}

//Function is called when you want to know the time it has taken
float dTime(){
  //Calculating the time inbetween when the function is called
  currentMillis = millis();
  diffMillis = currentMillis - startMillis;
  startMillis = currentMillis;

  //Returning the time in seconds
  return diffMillis / 1000;
}

//Function is called when you want to know how many pulses it has been
float dPulse(){
  //Calculating the pulses inbetween when the function is called
  currentPulse = rev;
  diffPulse = currentPulse - startPulse;
  startPulse = currentPulse;

  //Returning the pulses
  return diffPulse;
}

//Function is called when you want to know the velocity
float getVelocity(){ 
  //Calculating and returning the velocity in Cm/S
  return ((dPulse() / 100) * circumference) / dTime();
}

void loop() {
  //Includining what is crucial for MQTT services
  client.loop();

  //Setting the robots speed
  digitalWrite(dir, HIGH);
  analogWrite(pwm, enginePower);

  //If you have gotten a setValue from the website
  if (drive == true){
    //Calculating the new speed
    isValue = getVelocity();
    error = setValue - isValue;
    timeError += error * dTime();

    //Setting the new speed
    enginePower = (kP * error) + (kI * timeError);

    //Publishing the isValue to the MQTT topic
    client.publish("linus.kasper@abbindustrigymnasium.se/logger","linus " + String(isValue) + " /");

    //An if statement that can save the calculation in case of errors
    if (enginePower > 1023){
      enginePower = 1023;
    }
  }
}