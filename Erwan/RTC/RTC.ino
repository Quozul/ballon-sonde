#include <Wire.h>
#include "DS1307.h"

DS1307 clock; // créer l'horloge

float t = 24.7525; // temps restant en h
long int reset;

const float R1 = 300.0; // valeur de la première résistance
const float R2 = 180.0; // valeur de la seconde résistance
const float a = 2.500 + 2.100 + 2.100 + 2.500 + 2.000 + 2.100; // autonomie totale des piles en Ah

const float coeff = R2 / ( R1 + R2); // calcul du coefficient de réduction
const float req = (R1 * R2) / ( R1 + R2 ); // calcul de la résistance équivalente
const float res = 5 / 1024; // calcul la résolution de la borne analogique (V / unité)

const float tensionMax = 7.2; // tension maximale en V
const float tensionMin = 6.0; // défini la tension minimale en V

const int treuilPin = 2; // broche pour activer le treuil

void setup() {
    Serial.begin(9600);
    clock.begin();
    clock.fillByYMD(2019,4,2); // change la date (année, mois, numéro)
    clock.fillByHMS(8,30,0); // change l'heure (h,m,s)
    clock.fillDayOfWeek(TUE); // change le jour
    clock.setTime();
}

int daysinmonth(int mo) {
  switch (mo) {
    case 1:
      return 31;
    case 2:
      return 30;
    case 3:
      return 31;
    case 4:
      return 30;
    case 5:
      return 31;
    case 6:
      return 30;
    case 7:
      return 31;
    case 8:
      return 31;
    case 9:
      return 30;
    case 10:
      return 31;
    case 11:
      return 30;
    case 12:
      return 31;
  }
}

unsigned long timestamp() {
  clock.getTime();
  unsigned long i0 = (unsigned long)3600 * (unsigned long)24 * daysinmonth(clock.month);
  unsigned long i1 = (unsigned long)clock.month * i0;
  unsigned long i2 = (unsigned long)clock.year * i0 * (unsigned long)12;
  unsigned long seconds = (unsigned long)clock.second + (unsigned long)clock.minute*(unsigned long)60 + (unsigned long)clock.hour*(unsigned long)3600 + (unsigned long)clock.dayOfMonth*((unsigned long)3600*(unsigned long)24) + i1 + i2;
  return seconds;
}

void date(unsigned long timestamp) {
  clock.getTime();

  unsigned long annee = timestamp / ((unsigned long)12 * daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  timestamp -= annee * ((unsigned long)12 * daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  Serial.print(annee);
  Serial.print("/");

  unsigned long mois = timestamp / (daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  timestamp -= mois * (daysinmonth(clock.month) * (unsigned long)24 * (unsigned long)3600);
  Serial.print(mois);
  Serial.print("/");

  unsigned long jour = timestamp / ((unsigned long)24 * (unsigned long)3600);
  timestamp -= jour * ((unsigned long)24 * (unsigned long)3600);
  Serial.print(jour);
  Serial.print(" - ");

  unsigned long heure = timestamp / (unsigned long)3600;
  timestamp -= heure * (unsigned long)3600;
  Serial.print(heure);
  Serial.print(":");

  unsigned long mins = timestamp / (unsigned long)60;
  timestamp -= mins * (unsigned long)60;
  Serial.print(mins);
  Serial.print(":");

  Serial.println(timestamp);
}

const unsigned long secsindays = 3600L * 24L;
void autonomy(float t) {
  if (reset > millis())
    return;
    
  float days, hours, minutes;
  long int seconds = t * 3600;
  //Serial.println(seconds);
  
  days = seconds / secsindays;

  seconds -= days * secsindays;

  hours = seconds / (float)3600;
  seconds -= trunc(hours) * (float)3600;

  minutes = seconds / (float)60;
  seconds -= trunc(minutes) * (float)60;

  Serial.print(trunc(days), 0);
  Serial.print(" jour(s) ");
  Serial.print(trunc(hours), 0);
  Serial.print(":");
  Serial.print(trunc(minutes), 0);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.println(" ");
  
  /*char buffer [50];
  int i = sprintf (buffer, "%l jours %l : %l : %l", days, hours, minutes, seconds);
  for(int l= 0; l<=i; l++) 
    Serial.print(buffer[l]);*/
  Serial.println(" ");
  
  reset += 1000;
}

void loop() {
    //autonomy((float)getclockinseconds() / 3600.0 + (float)battery());
    unsigned long temps = battery() * 3600;
    //Serial.print(temps);
    //Serial.print(" - ");
  if (reset > millis())
    return;
    date(timestamp() + temps);
    //printTime();
  reset += 1000;
}

void recharge(float t) { // fonction qui affiche la date estimée à laquelle le ballon doit être rechargée
  if (reset > millis())
    return;
  
  float jours = trunc(t / 24);
  t -= jours * 24;
  float heures = trunc(t);
  t -= heures;
  float minutes = trunc(t * 60);
  t = (t * 60) - minutes;
  float secondes = t * 60;

  Serial.print("Recharge dans: ");
  Serial.print(jours, 0);
  Serial.print(" jour(s) ");
  Serial.print(heures, 0);
  Serial.print("h");
  Serial.print(minutes, 0);
  Serial.print(":");
  Serial.print(secondes, 4);
  Serial.println(" ");
  
  clock.getTime();
  Serial.print("Recharge le: ");
  Serial.print(clock.dayOfMonth + jours, 0);
  Serial.print("/");
  Serial.print(clock.month, DEC);
  Serial.print(" ");
  Serial.print(clock.hour + heures, 0);
  Serial.print("h");
  Serial.print(clock.minute + minutes, 0);
  Serial.print(":");
  Serial.print(clock.minute + secondes, 0);
  Serial.println("\n ");

  reset += 1000;
}

void printTime()
{
    clock.getTime();
    Serial.print(clock.hour, DEC);
    Serial.print(":");
    Serial.print(clock.minute, DEC);
    Serial.print(":");
    Serial.print(clock.second, DEC);
    Serial.print("  ");
    Serial.print(clock.month, DEC);
    Serial.print("/");
    Serial.print(clock.dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(clock.year+2000, DEC);
    Serial.print(" ");
    Serial.print(clock.dayOfMonth);
    Serial.print("*");
    switch (clock.dayOfWeek)// Friendly printout the weekday
    {
        case MON:
        Serial.print("MON");
        break;
        case TUE:
        Serial.print("TUE");
        break;
        case WED:
        Serial.print("WED");
        break;
        case THU:
        Serial.print("THU");
        break;
        case FRI:
        Serial.print("FRI");
        break;
        case SAT:
        Serial.print("SAT");
        break;
        case SUN:
        Serial.print("SUN");
        break;
    }
    Serial.println(" ");
}

float battery() {
  float rawcurr = analogRead(A1); // récupère la tension brute
  float u = rawcurr * (float)5 / 1024 / coeff; // calcul la tension réelle
  float i = u / req; // calcul l'intensité en A (u=ri)
  float t = a / i; // calcul l'autonomie en h
  
  if (reset <= millis()) { // affiche des valeurs toutes les 0.5 secondes
    /*Serial.println("");
    Serial.print(u, 7); // affiche la tension dans le moniteur de série avec 7 décimales
    Serial.println("V");
    Serial.print(t, 7); // affiche le temps restant avant décharge
    Serial.println("h");*/

    //reset = millis() + 500;
    
    //if (u < tensionMin)
      //Serial.println("Recharge nécessaire!");
  }
  
  if (u < tensionMin) {
    digitalWrite(treuilPin, HIGH);
  } else {
    digitalWrite(treuilPin, LOW);
  }

  return t;
}
