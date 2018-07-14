/*
  Projet de feu avec minuteur pour stand de tir
  Fondateur : Dimitri
  Développement : Dimitri, Sylvain

  12/07/2018 : Code de test de paramétrage du minuteur
  13/07/2018 : Lance 1 fois le programme complet avec x cycle I et y cycle II en utilisant les timers

*/

// LED
//// Pins
int LedPins[] = {7, 8};
int LedNb = 2;
//// Status
int LedState[] = {LOW, HIGH};

int Prog_val = 2;
// Programme 1 [NOT IMPLEMENTED]
// 1 Bip court + Pause 5s + (Bip long + Led R) 2sec x num Prog +
// [Pause 60s + Led R 7s + Led V 10s + Led R 7s ] x n cycles

// Programme 2 [DONE]
// 1 Bip court + Pause 5s + (Bip long + Led R) 2sec x num Prog +
// [Pause 60s + [Led R 7s + Led V 3s] x 5 + Led R 7s ] x n cycles

int int_Last     = 7 * 1000;
int int_Rest     = 10 * 1000;
int int_Inactive = 4 * 1000;
int int_Active   = 6 * 1000;


// [TO DO] Temporisation (3 ; 4 ; 6 ; 8 ; 10 ; 20)
// int Tempo     = 3;

// Cycles (1 ; 5 ; infini)
int CycleINb     = 2; // Global (Rest - Cycle II - Stop)
int CycleIINb    = 5; // Internal (Red - Green)

// Internal Parameters
int i = 0;
boolean debug = true;
unsigned long previousMillis = 0;






void setup() {
  // Start serial debugger
  Serial.begin(9600);

  if (debug) {
    Serial.println("Initialisation");
  }
  // Set the digital pin as output:
  for (int x = 0 ; x < LedNb ; x++) {
    int ThisPin = LedPins[x];
    pinMode(ThisPin, OUTPUT);
  }

  if (debug) {
    Serial.println("Test des panneaux leds (3 clignotements)");
  }
  // Leds blinks 2 times
  BlinkLeds(3, 500, 500);
}






void loop() {
  if (debug) {
    Serial.println("Début du programme");
  }
  while (i < 1) {
    LightProgram();
    i = 1;
  }
}






void LightProgram() {
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
