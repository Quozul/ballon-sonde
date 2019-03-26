

#include <TFTv2.h>

#include "seeed_bme680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#include <stdint.h>
#include <SPI.h>

#define IIC_ADDR  uint8_t(0x76)


/**  NOTE!!!!!!!!!!!!  Select the communication protocol correctly**/

Seeed_BME680 bme680(IIC_ADDR);  //IIC PROTOCOL 
//Seeed_BME680 bme680;             /* SPI PROTOCOL 
//Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);/*SPI PROTOCOL


void setup() {
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

  
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
   

}

void loop() {



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


    float mark[3]= {bme680.sensor_result_value.temperature, bme680.sensor_result_value.pressure / 100, bme680.sensor_result_value.humidity};

        // réinitialisation de l ecran
      Tft.drawHorizontalLine(0,50,2,BLACK);
      Tft.drawHorizontalLine(0,100,2,BLACK);
      Tft.drawHorizontalLine(0,150,2,BLACK);

      // affichage de la date
      Tft.drawString ("26/03 10:46  25%",0,5,2,RED);

      //affichage de la temperature
      Tft.drawString("Temperature =>",0,25,2,RED);
      Tft.drawFloat(mark[0],0,50,2,CYAN);

      Tft.drawString("Pression atmos.",0,75,2,RED);
      Tft.drawFloat(mark[1],0,100,2,CYAN);

      Tft.drawString("Taux d'humidite  ",0,125,2,RED);
      Tft.drawFloat(mark[2],0,150,2,CYAN);


      delay(5000);

}
