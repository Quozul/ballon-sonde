#include <TFTv2.h>

#include <Wire.h>
#include "MutichannelGasSensor.h"
#include "DS1307.h" // bibliothèque rtc

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

// variables rtc
DS1307 clock; // créer l'horloge

float t = 24.7525; // temps restant en h
long int reset;

const float R1 = 300.0; // valeur de la première résistance
const float R2 = 180.0; // valeur de la seconde résistance
const float Q = 2.500 + 2.100 + 2.100 + 2.500 + 2.000 + 2.100; // capacité totale des piles en Ah

const float coeff = R2 / ( R1 + R2); // calcul du coefficient de réduction
const float req = (R1 * R2) / ( R1 + R2 ); // calcul de la résistance équivalente
const float res = 5 / 1024; // calcul la résolution de la borne analogique (V / unité)

const float tensionMax = 7.2; // tension maximale (théorique) en V
const float tensionMin = 6.0; // défini la tension minimale en V

const int treuilPin = 23; // broche pour activer le treuil
// fin

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
    
    // rtc
    Serial.begin(9600);
    clock.begin(); // démarre l'horloge
    clock.fillByYMD(2019,4,02); // change la date (année, mois, numéro)
    clock.fillByHMS(8,30,0); // change l'heure (h,m,s)
    clock.fillDayOfWeek(TUE); // change le jour
    clock.setTime();
    pinMode(treuilPin, OUTPUT);
    // --

}

// fonctions rtc
int daysinmonth(int mo) { // fonction qui calcule le nombre de jours dans le mois
  if (mo <= 7) { // avant juillet
    if (mo % 2 == 0)
      return 30;
  } else { // après août
    if (mo % 2 == 1)
      return 30;
  }
  return 31;
}

unsigned long datetoseconds() { // convertis la date actuelle en secondes
  clock.getTime();
  unsigned long i0 = (unsigned long)3600 * (unsigned long)24 * daysinmonth(clock.month);
  unsigned long i1 = (unsigned long)clock.month * i0;
  unsigned long i2 = (unsigned long)clock.year * i0 * (unsigned long)12;
  unsigned long seconds = (unsigned long)clock.second + (unsigned long)clock.minute*(unsigned long)60 + (unsigned long)clock.hour*(unsigned long)3600 + (unsigned long)clock.dayOfMonth*((unsigned long)3600*(unsigned long)24) + i1 + i2;
  return seconds;
}

void secondstodate(unsigned long seconds) { // convertis la date donnée (en seconde) en date
  clock.getTime();

  unsigned long annee = seconds / ((unsigned long)12 * daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  seconds -= annee * ((unsigned long)12 * daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  Serial.print(annee);
  Serial.print("/");

  unsigned long mois = seconds / (daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  seconds -= mois * (daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  Serial.print(mois);
  Serial.print("/");

  unsigned long jour = seconds / ((unsigned long)24 * (unsigned long)3600);
  seconds -= jour * ((unsigned long)24 * (unsigned long)3600);
  Serial.print(jour);
  Serial.print(" - ");

  unsigned long heure = seconds / (unsigned long)3600;
  seconds -= heure * (unsigned long)3600;
  Serial.print(heure);
  Serial.print(":");

  unsigned long mins = seconds / (unsigned long)60;
  seconds -= mins * (unsigned long)60;
  Serial.print(mins);
  Serial.print(":");
  
  Tft.drawChar((mois / 10) + '0',0,5,2,WHITE);
  Tft.drawChar((mois % 10) + '0',12,5,2,WHITE);
  Tft.drawChar('/',24,5,2,WHITE);
  Tft.drawChar((jour / 10) + '0',36,5,2,WHITE);
  Tft.drawChar((jour % 10) + '0',48,5,2,WHITE);
  
  Tft.drawChar((heure / 10) + '0',72,5,2,WHITE);
  Tft.drawChar((heure % 10) + '0',84,5,2,WHITE);
  Tft.drawChar(':',96,5,2,WHITE);
  Tft.drawChar((mins / 10) + '0',108,5,2,WHITE);
  Tft.drawChar((mins % 10) + '0',120,5,2,WHITE);

  Serial.print(seconds);
}
// fin

void loop() 
{

      Tft.fillRectangle(0, 50, 240,20,BLACK);
      Tft.fillRectangle(0, 100, 240,20,BLACK);
      Tft.fillRectangle(0, 150, 240,20,BLACK);
      Tft.fillRectangle(0, 200, 240,20,BLACK);
      Tft.fillRectangle(0, 250, 240,20,BLACK);
  
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
   

      //Tft.drawString ("28/03 15:22 25%",0,5,2,WHITE);
      
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

    // rtc
  
    Tft.fillRectangle(0, 5, 240,20,BLACK);
    float rawcurr = analogRead(A10); // récupère la tension brute
    float u = rawcurr * (float)5 / 1024 / coeff; // calcul la tension réelle
    float i = u / req; // calcul l'intensité en A (u=ri)
    unsigned long temps = Q / i * (float)3600; // calcul l'autonomie en s
  
    //if (reset > millis()) return; // le code qui suit cette ligne ne va s'exécuter que tout les secondes
    secondstodate(datetoseconds() + temps);

    float per = min((u - tensionMin) / (tensionMax - tensionMin) * 100, 100);
    Serial.print(" ");
    Serial.print(per, DEC);
    Serial.println("%");
    
    Tft.drawFloat(per,165,5,2,WHITE);
    Tft.drawChar('%',225,5,2,WHITE);

    if (u < 6.0)
      digitalWrite(treuilPin, HIGH);
    else
      digitalWrite(treuilPin, LOW);
    
    delay(5000);
    //reset += 1000;
    // --
}
