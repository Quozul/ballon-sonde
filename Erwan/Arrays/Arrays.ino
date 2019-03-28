int valeurs[10] = {20, 50, 1, 5};
char noms[10][10] = {
  {'T', 'e', 'm', 'p', '\0'},
  {'H', 'u', 'm', 'm', '\0'},
  {'P', 'r', 'e', 's', '\0'},
};

void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(valeurs) / sizeof(int));
}

void loop() {
  for ( int i = 0; i < sizeof(valeurs) / sizeof(int); i++ ) {
    Serial.print(noms[i]);
    Serial.print(" : ");
    Serial.println(valeurs[i]);
  }
}
