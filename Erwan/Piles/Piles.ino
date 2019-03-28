long int reset;

const float R1 = 300.0; // valeur de la première résistance
const float R2 = 180.0; // valeur de la seconde résistance
const float a = 2.500+2.000+2.100+2.500+2.100+2.100; // autonomie totale des piles en Ah

const float coeff = R2 / ( R1 + R2); // calcul du coefficient de réduction
const float req = (R1 * R2) / ( R1 + R2 ); // calcul de la résistance équivalente
const float res = 5 / 1024; // calcul la résolution de la borne analogique (V / unité)

const float tensionMax = 7.2; // tension maximale en V
const float tensionMin = 6; // défini la tension minimale en V

const int treuilPin = 2; // broche pour activer le treuil

void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(treuilPin, OUTPUT);
}

void loop() {
  battery();
}

float battery() {
  float rawcurr = analogRead(A1); // récupère la tension brute
  float u = rawcurr * (float)5 / 1024 / coeff; // calcul la tension réelle
  float i = u / req; // calcul l'intensité en A (u=ri)
  float t = a / i; // calcul l'autonomie en h
  
  if (reset <= millis()) { // affiche des valeurs toutes les 0.5 secondes
    Serial.println("");
    Serial.print(rawcurr, 0); // affiche la tension dans le moniteur de série avec 7 décimales
    Serial.println("");
    Serial.print(u, 7); // affiche la tension dans le moniteur de série avec 7 décimales
    Serial.println("V");
    Serial.print(t, 7); // affiche le temps restant avant décharge
    Serial.println("h");

    reset = millis() + 500;
    
    if (u < tensionMin)
      Serial.println("Recharge nécessaire!");
  }
  
  if (u < tensionMin) {
    digitalWrite(treuilPin, HIGH);
  } else {
    digitalWrite(treuilPin, LOW);
  }
  
  return t;
}
