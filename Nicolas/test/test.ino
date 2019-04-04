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

//convertion d une chaine "dynamique" en char*
char* string2char(String chaine){
    if(chaine.length()!=0){
        char *p = const_cast<char*>(chaine.c_str());
        return p;
    }
}

void afficher_mesure(String type, float val, String unite, int line)
{
      
    
      //display debug console
      Serial.println(type + ": ");
      Serial.println(String(val));
      Serial.println("\n");
      
      // reinitialisation de la ligne
      Tft.drawFloat(bme680.sensor_result_value.temperature,0,50,2,BLACK);

      //affiche le type, la valeur et l unite sur l ecran le tout convertit de string a char*
      Tft.drawString(string2char(type + ": "), 0, line, 2, RED);
      Tft.drawString(string2char(String(val) + " " + unite), 0, line+25, 2, CYAN);
    
}

void loop() {

    //initialisation de la premiere ligne de donnees
    int line = 25;
    
    // lecture des valeurs
    if (bme680.read_sensor_data()) 
    {
      Serial.println("Erreur de lécture:(");
      return;
    }

    // affichage de la date
    Tft.drawString(string2char("28/03 14:30  25%"),0,5,2,RED);
    

    //affichage des mesures toutes les 50 lignes
    afficher_mesure("temperature", bme680.sensor_result_value.temperature, "C", line);
    line = line + 50;
    afficher_mesure("pression", bme680.sensor_result_value.pressure / 100, "hPa", line);
    line = line + 50;
    afficher_mesure("humidite", bme680.sensor_result_value.humidity, "%", line);

    //tempo 5s
    delay(5000);

}
