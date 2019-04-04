#include <Wire.h>
#define Addr 0x50
int capteur03=A0;
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

void setup() {
  
  Serial.begin(9600);
  pinMode(8,INPUT);
  starttime = millis(); 
  
   // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);

}
void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
     

  Serial.println("lowpulseoccupancy");   
    Serial.println(lowpulseoccupancy);
       Serial.println("");
     Serial.println("ratio");
          Serial.println(ratio);
    Serial.println("");
    Serial.println("concentration");
      Serial.print(concentration);
      Serial.println("ppm");
    lowpulseoccupancy = 0;
    starttime = millis();
    Serial.println("");
  }
  {
  unsigned int data[2];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to 12-bits
  int raw_adc = ((data[0] & 0x0F) * 256) + data[1];
  float ppm = (1.99 * raw_adc) / 4095.0 + 0.01;

  // Output data to serial monitor
  Serial.print("Ozone Concentration : ");
  Serial.print(ppm);
  Serial.println(" ppm");
  delay(3000);
  Serial.println("");
}
}
