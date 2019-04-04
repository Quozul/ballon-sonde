#include <Wire.h>
#include "MutichannelGasSensor.h"
int green = 6;
int yellow = 4;
int red = 2;

void setup()
{
    pinMode(red,OUTPUT);
    pinMode(yellow,OUTPUT);
    pinMode(green,OUTPUT);
    
    Serial.begin(115200);
    Serial.println("power on!");
    gas.begin(0x04);
    gas.powerOn();
    Serial.print("Firmware Version = ");
    Serial.println(gas.getVersion());

    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
    digitalWrite(yellow,LOW);
}

void loop()
{
    float c;

    c = gas.measure_NO2();
    Serial.print("Taux de NO2 dans l'air : ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");

    if(gas.measure_NO2() <= 0.9){
    digitalWrite(green,LOW);
    digitalWrite(yellow,HIGH);
    digitalWrite(red,LOW);
  }else if(gas.measure_NO2() >= 0.10){
    digitalWrite(green,LOW);
    digitalWrite(yellow,LOW);
    digitalWrite(red,HIGH);
  }
    
    delay(1000);
}
