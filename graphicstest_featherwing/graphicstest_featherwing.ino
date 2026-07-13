/***************************************************
  This is our GFX example for the Adafruit ILI9341 TFT FeatherWing
  ----> http://www.adafruit.com/products/3315

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Fonts/FreeSerif24pt7b.h>

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef __AVR_ATmega32U4__
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef ARDUINO_SAMD_FEATHER_M0
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif
#ifdef ARDUINO_NRF52_FEATHER /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

long freq = 99999990;
long freqOld = 0;
char szFreq[16];

void updateDisplay(long freq)
{
    if (freq != freqOld)
    {
      int megs = freq / 1000000;
      int kilos = (freq - (megs * 1000000)) / 1000;
      int hundreds = freq - (megs * 1000000) - (kilos * 1000);

      sprintf(szFreq, "%3d.%03d.%03d", megs, kilos, hundreds);
      freqOld = freq;

      int16_t x1, y1;
      uint16_t w, h;
      
      tft.getTextBounds(szFreq, 8, 100, &x1, &y1, &w, &h);
      tft.fillRect(x1-4, y1-4, w+8, h+8, ILI9341_BLACK);
      tft.drawRect(x1-4, y1-4, w+8, h+8, ILI9341_BLUE);
      tft.setCursor(8, 100);
      tft.setTextColor(ILI9341_YELLOW);
      tft.print(szFreq);
    }
}

void setup() 
{
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);

  tft.setFont(&FreeSerif24pt7b);
}


void loop(void) 
{
    updateDisplay(freq+=1000000);
    yield();
}

