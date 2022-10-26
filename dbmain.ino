/*
DB
*/
#include <LiquidCrystal.h>
#define RS 2
#define E 3
#define D4 8
#define D5 7
#define D6 6
#define D7 5
#define RED 11
#define GRN 10
#define BLU 9

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
float val = 0;
float error = 0;


void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLU, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0,0);
  val = analogRead(A0) * (5.0 / 1023.0) + error;
  lcd.print(val);
  delay(250);
  
}

