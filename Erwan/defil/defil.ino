#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

int page = 0;
/*const int button_size = 15;
const int previous_page[2] = {20, 290};
const int next_page[2] = {215, 290};*/

unsigned long screen_update;
unsigned long next_page;

const int elements_in_page = 1;



void setup() {
  Serial.begin(9600);
  Tft.TFTinit();  //init TFT library
}

bool checkCollision(int x1, int y1, int s, int x2, int y2) {
  if (x1 < x2 && x2 < x1+s && y1 < y2 && y2 < y1+s)
    return true;
  return false;
}

void loop() {
  /* Recupère la position du doigt et change la page
   * 
   * Point p = ts.getPoint();
  
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

  Tft.fillCircle(previous_page[0], previous_page[1], button_size, CYAN);
  Tft.fillCircle(next_page[0], next_page[1], button_size, CYAN);

  if (checkCollision(previous_page[0], previous_page[1],button_size , p.x, p.y))
    page--;

  if (checkCollision(next_page[0], next_page[1],button_size , p.x, p.y))
    page++;

  Serial.print("x: ");
  Serial.print(p.x);
  Serial.print(" y: ");
  Serial.println(p.y);*/

  /*
   * Change la page toutes les 10 secondes
   */

  if (next_page < millis()) {
    page++;
    
    if (page >= 9)
      page = 0;

    next_page = millis() + 10000;
  }
  
  draw_page(page);
}

void draw_page(int page) {
  if (screen_update > millis())
    return;

  // En-tête
  int i;
  for (i = 0 ; i < 320 ; i += 20) {
    Tft.fillRectangle(0, i, 240, 20, BLACK);
  }
  
  Tft.drawChar(page + '0', 0, 5, 2, WHITE);

  // Corps de la page
  
  
  screen_update = millis() + 1000;
}
