#include "U8glib.h"

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

int A_read[2];



void setup(void) {
  u8g.setRot180(); // flip screen, if required
}

void loop(void) {
  A_read[0] = map(analogRead(0), 0, 1023, 1, 80);
  A_read[1] = map(analogRead(1), 0, 1023, 1, 80);
  
  // picture loop
  u8g.firstPage();  
  do {
    draw(A_read[0], A_read[1]);
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(100);
}


void draw(int text_st1, int text_st2) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  // call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.setPrintPos(0, 22); 
  String string_txt1 = String(text_st1);
  u8g.print(string_txt1);
  u8g.setPrintPos(50, 22); 
  String string_txt2 = String(text_st2);
  u8g.print(string_txt2);
}
