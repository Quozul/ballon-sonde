#include <Wire.h>
#include "DS1307.h"

DS1307 clock; // créer l'horloge

float t = 24.7525; // temps restant en h
long int reset;

const float R1 = 100.0; // valeur de la première résistance
const float R2 = 500.0; // valeur de la seconde résistance
const float a = 2.850; // autonomie totale des piles en Ah

const float coeff = R2 / ( R1 + R2); // calcul du coefficient de réduction
const float req = (R1 * R2) / ( R1 + R2 ); // calcul de la résistance équivalente
const float res = 5 / 1024; // calcul la résolution de la borne analogique (V / unité)

const float tensionMax = 6.0; // tension maximale en V
const float tensionMin = 5.5; // défini la tension minimale en V

const int treuilPin = 2; // broche pour activer le treuil

void setup() {
    Serial.begin(9600);
    clock.begin();
    clock.fillByYMD(2019,3,26); // change la date (année, mois, numéro)
    clock.fillByHMS(9,20,0); // change l'heure (h,m,s)
    clock.fillDayOfWeek(THU); // change le jour
    clock.setTime();
}

unsigned long getclockinseconds() {
  clock.getTime();
  unsigned long seconds;
  seconds += clock.second;
  seconds += clock.minute * 60;
  seconds += clock.hour * 3600;
  return seconds;
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
    autonomy((float)getclockinseconds() / 3600.0 + (float)battery());
    //Serial.println(getclockinseconds(), DEC);
    //printTime();
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
    Serial.println("");
    Serial.print(u, 7); // affiche la tension dans le moniteur de série avec 7 décimales
    Serial.println("V");
    Serial.print(t, 7); // affiche le temps restant avant décharge
    Serial.println("h");

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
