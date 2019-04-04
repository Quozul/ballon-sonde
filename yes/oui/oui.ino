#include <TFTv2.h>

#include <Wire.h>
#include "MutichannelGasSensor.h"

#include "seeed_bme680.h"
int green = 7;
int yellow = 5;
int red = 3;
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#include <stdint.h>
#include <SPI.h>
float no2 = gas.measure_NO2();
#define IIC_ADDR  uint8_t(0x76)

/**  NOTE!!!!!!!!!!!!  Select the communication protocol correctly**/

Seeed_BME680 bme680(IIC_ADDR);  //IIC PROTOCOL 
//Seeed_BME680 bme680;             /* SPI PROTOCOL 
//Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);/*SPI PROTOCOL


void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

digitalWrite(green,LOW);
digitalWrite(red,LOW);
digitalWrite(yellow,LOW);  

/**digitalWrite(green,HIGH);
digitalWrite(red,HIGH);
digitalWrite(yellow,HIGH);**/ 

  pinMode(red,OUTPUT);
  pinMode(yellow,OUTPUT);
  pinMode(green,OUTPUT);
  //bme680.init(); 
    
  //bme680.read_sensor_data();  

  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial start!!!");
  delay(100);
  while (!bme680.init()) 
  {
    Serial.println("bme680 init failed ! can't find device!");
  delay(10000);
  }
  
    gas.begin(0x04);//the default I2C address of the slave is 0x04
    gas.powerOn();
    
   

}

void loop() 
{

  
  if (bme680.read_sensor_data()) 
  {
    Serial.println("Erreur de lécture:(");
    return;
  }
  Serial.print("Temperature → ");
  Serial.print(bme680.sensor_result_value.temperature);
  Serial.println(" C");

  Serial.print("Pression atmospherique → ");
  Serial.print(bme680.sensor_result_value.pressure /100);
  Serial.println(" hPa");

  Serial.print("Taux d'humidite → ");
  Serial.print(bme680.sensor_result_value.humidity);
  Serial.println(" %\n");
  Serial.println("");
  
  Serial.print("azote → ");
  Serial.print(gas.measure_NO2());
  Serial.println("");
    
float mark[6]= {bme680.sensor_result_value.temperature, bme680.sensor_result_value.pressure / 100, bme680.sensor_result_value.humidity,50,gas.measure_NO2(),136.24};

  if(gas.measure_NO2() <= 0.9){
  digitalWrite(green,HIGH);
  digitalWrite(yellow,LOW);
  digitalWrite(red,LOW);
}else if(gas.measure_NO2() > 0.10){
  digitalWrite(green,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(red,HIGH);
}
   
    if (1 == 1)
    {

      Tft.drawString ("28/03 15:22 25%",0,5,2,WHITE);
      
      Tft.drawString("Temperature",0,25,2,WHITE);  // draw char: 'S', (0, 0), size: 1, color: RED
      
      Tft.drawFloat(mark[0],0,50,2,CYAN);       // draw string: "hello", (0, 180), size: 3, color: CYAN
    
      Tft.drawString("Pression atmos.",0,75,2,WHITE);             // draw char: 'E', (10, 10), size: 2, color: BLUE
    
      Tft.drawFloat(mark[1] ,0,100,2,CYAN);       // draw string: "hello", (0, 180), size: 3, color: CYAN
    
      Tft.drawString("Taux d'humidite  ",0,125,2,WHITE);            // draw char: 'E', (20, 40), size: 3, color: GREEN

      Tft.drawFloat(mark[2],0,150,2,CYAN);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
      
      Tft.drawString("Ozone",0,175,2,WHITE);             // draw char: 'E', (10, 10), size: 2, color: BLUE
    
      Tft.drawFloat(mark[3],0,200,2,CYAN);       // draw string: "hello", (0, 180), size: 3, color: CYAN
    
      Tft.drawString("Azote",0,225,2,WHITE);            // draw char: 'E', (20, 40), size: 3, color: GREEN
    
      Tft.drawFloat(mark[4],0,250,2,CYAN);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
      
      Tft.drawString("Particule fines",0,275,2,WHITE);            // draw char: 'E', (20, 40), size: 3, color: GREEN
    
      Tft.drawFloat(mark[5],0,300,2,CYAN);    // draw string: "world!!", (80, 230), size: 4, color: WHITE

      delay(5000);
      
      Tft.drawFloat(mark[0],0,50,2,BLACK);       // draw string: "hello", (0, 180), size: 3, color: CYAN   
   
      Tft.drawFloat(mark[1] ,0,100,2,BLACK);       // draw string: "hello", (0, 180), size: 3, color: CYAN
      
      Tft.drawFloat(mark[2],0,150,2,BLACK);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
    
      Tft.drawFloat(mark[3] ,0,200,2,BLACK);       // draw string: "hello", (0, 180), size: 3, color: CYAN
        
      Tft.drawFloat(mark[4],0,250,2,BLACK);    // draw string: "world!!", (80, 230), size: 4, color: WHITE

      Tft.drawFloat(mark[5],0,300,2,BLACK);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
      delay(10);
      
    }
    
}
