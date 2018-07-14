#include "U8glib.h"

// Display variables
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

// Input variables

String text_st3 = "";

// Pins
int LedPins[] = {7, 8};
int LedNb = 2;
int InputPins[] = {0, 1};
int InputVal[] = {0, 0};
int InputNb = 2;

// Values
int Prog_val = 2;
int int_Last     = 7 * 1000;
int int_Rest     = 10 * 1000;
int int_Inactive = 4 * 1000;
int int_Active   = 6 * 1000;
// int Tempo     = 3;
int CycleINb     = 2; // Global (Rest - Cycle II - Stop)
int CycleIINb    = 5; // Internal (Red - Green)

// Programming data
int i = 0;
boolean debug = true;
unsigned long previousMillis = 0;






void setup(void) {
  // Start serial debugger
  Serial.begin(9600);

  u8g.setRot180(); // flip screen, if required
  pinMode(2, INPUT_PULLUP);

  // Set the digital pin as output:
  for (int x = 0 ; x < LedNb ; x++) {
    int ThisPin = LedPins[x];
    pinMode(ThisPin, OUTPUT);
  }

  // Leds blinks 2 times
  BlinkLeds(3, 500, 500);
}






void loop(void) {
  // Read Control
  for (int i = 0 ; i < InputNb ; i++) {
    InputVal[i] = map(analogRead(InputPins[i]), 0, 1023, 1, 60);
  }

  // Check Button
  int button_val = digitalRead(2);
  if (digitalRead(2) == LOW) {
    text_st3 = "Sending...";
  } else {
    text_st3 = "";
  }

  // picture loop
  draw(InputVal[0], InputVal[1], text_st3);



  if (text_st3 == "Sending...") {
    delay(1000); // Script d'E/R du signal
    draw(InputVal[0], InputVal[1], "OK");
    while (i < 1) {
      LightProgram(InputVal[0] * 1000, InputVal[1] * 1000);
      i = 1;
    }
    i = 0;
  }

  // rebuild the picture after some delay
  delay(50);

}






void draw(int text_st1, int text_st2, String text_st3) {
  u8g.firstPage();
  do {
    // graphic commands to redraw the complete screen should be placed here
    u8g.setFont(u8g_font_profont12);
    // call procedure from base class, http://arduino.cc/en/Serial/Print

    u8g.setPrintPos(0, 10);
    u8g.print("t_Rep");
    u8g.setPrintPos(0, 25);
    String string_txt1 = String(text_st1);
    u8g.print(string_txt1);

    u8g.setPrintPos(40, 10);
    u8g.print("t_Act");
    u8g.setPrintPos(40, 25);
    String string_txt2 = String(text_st2);

    u8g.print(string_txt2);
    u8g.setPrintPos(60, 25);
    u8g.print(text_st3);
  } while (u8g.nextPage());
}


void LightProgram(int int_Rest, int int_Active) {
  unsigned long last_event_time = millis();
  int event_count = 0;
  int last_event = 4;
  int led_stat = 0;
  int cycleI_count = 0;
  int cycleII_count = 0;
  int y = 0;
  int x = 0;

  if (debug) {
    Serial.print("Event : ");
    Serial.print(event_count);
    Serial.print(" | CycleI : ");
    Serial.print(y);
    Serial.print(" | CycleII : ");
    Serial.println(x);
  }

  while (event_count < last_event) {
    if (event_count == 0 && millis() - last_event_time >= 5000) {
      BlinkLeds(Prog_val, 2000, 500);
      event_count++;
      last_event_time = millis();
      if (debug) {
        Serial.print("Event : ");
        Serial.print(event_count);
        Serial.print(" | CycleI : ");
        Serial.print(y);
        Serial.print(" | CycleII : ");
        Serial.println(x);
      }
    }

    if (event_count == 1) {
      for (int y = 0 ; y < CycleINb ; y++) {
        event_count = 1;
        while (event_count == 1) {
          if (millis() - last_event_time >= int_Rest) {
            event_count++;
            last_event_time = millis();

            if (debug) {
              Serial.print("Event : ");
              Serial.print(event_count);
              Serial.print(" | CycleI : ");
              Serial.print(y);
              Serial.print(" | CycleII : ");
              Serial.println(x);
            }
          }
        }

        for (int x = 0 ; x < CycleIINb ; x++) {
          event_count = 2;

          while (event_count == 2) {
            if (led_stat == 0) {
              digitalWrite(LedPins[0], HIGH);
              led_stat = 1;
              last_event_time = millis();
            }

            if (led_stat == 1 && millis() - last_event_time >= int_Inactive) {
              digitalWrite(LedPins[0], LOW);
              led_stat = 0;
              event_count++;
              last_event_time = millis();

              if (debug) {
                Serial.print("Event : ");
                Serial.print(event_count);
                Serial.print(" | CycleI : ");
                Serial.print(y);
                Serial.print(" | CycleII : ");
                Serial.println(x);
              }
            }
          }

          while (event_count == 3) {
            if (led_stat == 0) {
              digitalWrite(LedPins[1], HIGH);
              led_stat = 1;
              last_event_time = millis();
            }
            if (led_stat == 1 && millis() - last_event_time >= int_Active) {
              digitalWrite(LedPins[1], LOW);
              led_stat = 0;
              event_count++;
              last_event_time = millis();
              if (debug) {
                Serial.print("Event : ");
                Serial.print(event_count);
                Serial.print(" | CycleI : ");
                Serial.print(y);
                Serial.print(" | CycleII : ");
                Serial.println(x);
              }
            }
          }
        }

        while (event_count == 4) {
          if (led_stat == 0) {
            digitalWrite(LedPins[0], HIGH);
            led_stat = 1;
            last_event_time = millis();
          }
          if (led_stat == 1 && millis() - last_event_time >= int_Last) {
            digitalWrite(LedPins[0], LOW);
            led_stat = 0;
            event_count++;
            last_event_time = millis();

            if (debug) {
              Serial.print("Event : ");
              Serial.print(event_count);
              Serial.print(" | CycleI : ");
              Serial.print(y);
              Serial.print(" | CycleII : ");
              Serial.println(x);
            }
          }
        }
      }
    }
  }
}






void BlinkLeds(int times, int on_time, int off_time) {
  for (int y = 0 ; y < times ; y++) {
    LedsOn();
    delay(on_time);
    LedsOff();
    delay(off_time);
  }
}


void LedsOn() {
  for (int x = 0 ; x < LedNb ; x++) {
    int ThisPin = LedPins[x];
    digitalWrite(ThisPin, HIGH);
  }
}


void LedsOff() {
  for (int x = 0 ; x < LedNb ; x++) {
    int ThisPin = LedPins[x];
    digitalWrite(ThisPin, LOW);
  }
}
