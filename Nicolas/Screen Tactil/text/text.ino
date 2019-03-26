/*  draw text's APP
    drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
    drawString(char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
*/

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

int data[100];

float mark[3]= {22, 1027, 41};



void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library
    
    Tft.drawString("Temp",0,0,1,RED);                // draw char: 'S', (0, 0), size: 1, color: RED
    
    Tft.drawNumber("22.60",0,10,1,BLUE);             // draw char: 'E', (10, 10), size: 2, color: BLUE
    
    Tft.drawString("Press",0,40,1,GREEN);            // draw char: 'E', (20, 40), size: 3, color: GREEN
    
    Tft.drawNumber("1027.02",0,80,1,YELLOW);           // draw char: 'E', (30, 80), size: 4, color: YELLOW
    
    Tft.drawString("hum",0,120,1,YELLOW);          // draw char: 'D', (40, 120), size: 4, color: YELLOW

    Tft.drawNumber("39.72",0,150,1,GREEN);
    
    Serial.begin(9600);
}

void loop()
{
  Serial.print("Voici les valeurs");// print first element of an array

//Serial.println("%d", mark[0]);

// print ith element of an array
//Serial.println("%d", mark[1]);

Serial.print(mark[3],"%d");
}
