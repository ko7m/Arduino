 #include "U8g2lib.h"

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);



void draw(void) {
//  u8g2.setFont(u8g_font_unifont); 
  u8g2.setFont(u8g2_font_unifont_tf); 
//  u8g2.setFont(u8g2_font_ncenB14_tr);
//  u8g2.setFont(u8g2_font_unifont_t_symbols);
//  u8g2.drawUTF8(5, 20, "Snowman: ☃");  
//  u8g2.drawStr(0, 15, "Hello World!");
//  u8g2.drawStr(0, 15, "A: 14.060000+010");
//  u8g2.drawStr(0, 30, "CWH 22 WPM");
//  u8g2.drawStr(0, 15, "         1");
//  u8g2.drawStr(0, 30, "1234567890123456");
  u8g2.drawStr(0, 15, "12345678901234567890");
  u8g2.drawStr(0, 30, "12345678901234567890");
}

void setup(void) {
    Serial.begin(115200);
    Serial.println("Hello World!");
    u8g2.begin();
    u8g2.clear();
//    u8g2.firstPage();
//    do {
//        draw();
//    } while( u8g2.nextPage() );
//u8g2.setBufferCurrTileRow(0);       // let y=0 be the topmost row of the buffer
//u8g2.clearBuffer();
//u8g2.setFont(u8g2_font_unifont_tf);
//u8g2.setFont(u8g2_font_profont12_mf);
//u8g2.drawStr(0, 8, "Hello World!");
//u8g2.drawBox(0, 0, 128, 32);
//u8g2.drawStr(0, 8, "A: 14.060000+010");
//u8g2.drawStr(0, 8, "12345678901234567890");
//u8g2.setBufferCurrTileRow(1);    // write the buffer to tile row 2 (y=16) on the display
//u8g2.sendBuffer();
//u8g2.clearBuffer();
//u8g2.drawStr(0, 16, "CWH 22 WPM");
//u8g2.drawStr(0, 16, "12345678901234567890");
//u8g2.setBufferCurrTileRow(3);   // write the same buffer to tile row 4 (y=32) on the display
//u8g2.sendBuffer();
u8g2.firstPage();
do {
    u8g2.setFont(u8g2_font_profont12_mf);
    Serial.print("Max height ");
    Serial.println(u8g2.getMaxCharHeight());
    Serial.print("Max width ");
    Serial.println(u8g2.getMaxCharWidth());
    u8g2.drawStr(0, 15, "A: 14.060000+010");
    u8g2.drawStr(0, 31, "CWH 22 WPM");
} while ( u8g2.nextPage() );
delay(1000);
u8g2.clear();
u8g2.firstPage();
do {
    u8g2.setFont(u8g2_font_profont12_mf);
    u8g2.drawStr(0, 15, "12345678901234567890");
    u8g2.drawStr(0, 31, "12345678901234567890");
} while ( u8g2.nextPage() );
}

void loop(void) {

}
