#include <Wire.h>
#include "DS1307.h"

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

const int treuilPin = 2; // broche pour activer le treuil

void setup() {
    Serial.begin(9600);
    clock.begin(); // démarre l'horloge
    clock.fillByYMD(2019,4,02); // change la date (année, mois, numéro)
    clock.fillByHMS(8,30,0); // change l'heure (h,m,s)
    clock.fillDayOfWeek(TUE); // change le jour
    clock.setTime();
}

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

  Serial.println(seconds);
}

void loop() {
  float rawcurr = analogRead(A1); // récupère la tension brute
  float u = rawcurr * (float)5 / 1024 / coeff; // calcul la tension réelle
  float i = u / req; // calcul l'intensité en A (u=ri)
  unsigned long temps = Q / i * (float)3600; // calcul l'autonomie en s
  
  if (reset > millis()) return; // le code qui suit cette ligne ne va s'exécuter que tout les secondes

  secondstodate(datetoseconds() + temps);
  float per = (u - tensionMin) / (tensionMax - tensionMin) * 100;
  Serial.print(" ");
  Serial.print(per, DEC);
  Serial.println("%");
  
  reset += 1000;
}
