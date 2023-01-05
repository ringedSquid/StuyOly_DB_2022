/* INITIAL DATA TESTING SCRIPT */
#include <LiquidCrystal.h>
#define READPIN A1
const int RS=2, E=3, D4=5, D5=6, D6=7, D7=8; //LCD Pins
const int RED=11, GRN=10, BLU=9; //LED Pins
const int TOGGLE = 12;
/*Weight ranges in grams, R1 <= R2*/
const double R1 = 50;
const double R2 = 250;
const double offset = 0; //ofsett reading for sensor
const int TESTS = 2560;
; //amount of times for the microcontroller to measure data from the sensor. Its all averaged out

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(TOGGLE, INPUT);
  lcd.begin(16, 12);
  lcd.setCursor(0, 0);
  lcd.print("DBMAIN_ANALOG");
}

void loop() {
  double finalsum = 0;
  boolean interrupt = false; //button pressed?
  while (interrupt == false) {
    digitalWrite(RED, LOW);
    digitalWrite(GRN, LOW);
    digitalWrite(BLU, LOW);
    if (digitalRead(TOGGLE) == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Measuring...");
      lcd.setCursor(0, 1);
      for (int i=0; i<TESTS; i++) {
        finalsum += analogRead(READPIN) - offset;
      }
      finalsum /= TESTS;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vd: ");
      lcd.print(finalsum);
      lcd.setCursor(0, 1);
      lcd.print("Vr: ");
      lcd.print(finalsum*(5/1023.0));
      interrupt = true;
    }
  }
}
