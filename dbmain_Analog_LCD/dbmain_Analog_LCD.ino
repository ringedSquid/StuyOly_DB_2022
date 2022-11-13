#include <LiquidCrystal.h>
#define TESTS 5
const int RS=2, E=3, D4=8, D5=7, D6=6, D7=5; //LCD pins
const int RED=11, GRN=10, BLU=9, TOGGLE=12;
const float R1 = 50, R2 = 250; //Weight ranges for LEDs
const double offset = 416 - ((416-198)+16);
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

byte bar[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GRN,OUTPUT);
  pinMode(BLU,OUTPUT);
  pinMode(TOGGLE,INPUT);
  lcd.begin(16,2);
  lcd.createChar(0, bar);
  lcd.setCursor(0,0);
  lcd.print("DBMAIN_ANALOG");
  Serial.begin(9600);
}

void loop() {
  double finalsum = 0;
  double mass = 0;
  boolean wait = true;
  while (wait == true) {
    digitalWrite(RED, LOW);
    digitalWrite(GRN, LOW);
    digitalWrite(BLU, LOW);
    if (digitalRead(TOGGLE) == HIGH) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("MEASURING...");
      lcd.setCursor(0,1);
      for (int i = 0; i<TESTS; i++) {
        finalsum += avg_measure(500);
        if (i % 7 == 0) {
          lcd.write(byte(0));
        }
      }
      finalsum /= TESTS;
      Serial.println(finalsum);
      mass = solve(finalsum);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Voltage: ");
      lcd.print(finalsum*(5/1023.0));
      lcd.print("v");
      lcd.setCursor(0,1);
      lcd.print("Mass: ");
      lcd.print(mass);
      lcd.print("g");
      if (mass < R1) {
        digitalWrite(RED, HIGH);
      }
      else if ((mass > R1) && (mass < R2)) {
        digitalWrite(GRN, HIGH);
      }
      else {
        digitalWrite(BLU, HIGH);
      }
      wait = false;
    }
  }
   
  
}

double avg_measure(int times) {
  double finalsum = 0;
  for (int i=0; i<times; i++) {
    finalsum += (analogRead(A0)+analogRead(A1)+analogRead(A2))/3 - offset;
  }
  return finalsum/times;
}

/* Solving polynomial of degree 6
 *  202 + 0.0673x + (1.61*10^-3x^2) + (-4.79*10^-6x^3) + (4.99*10^-9x^4) + (-1.75*10^-12x^5) + (-2.97*10^-17x^6)
 *  DERIVATIVE
 *  0.0673 + 0.00322 x - 0.00001437 x^2 + 1.996*10^-8 x^3 - 8.75*10^-12 x^4 - 1.7820000000000004*10^-16 x^5
*/

double fx(double x) { //og equation
  return (202) + (0.0673*x) + (1.61*pow(10,-3)*pow(x,2)) + (-4.79*pow(10,-6)*pow(x,3)) + (4.99*pow(10,-9)*pow(x,4)) + (-1.75*pow(10,-12)*pow(x,5)) + (-2.97*pow(10,-17)*pow(x,6));
}

double fx1(double x) { //derivative
  return (0.0673) + (0.00322*x) - (0.00001437*pow(x,2)) + (1.996*pow(10,-8)*pow(x,3)) - (8.75*pow(10,-12)*pow(x,4)) - (1.782*pow(10,-16)*pow(x,5));
}

double solve(double k) { 
  double x = 100;
  for (int i=0; i<30; i++) {
    x = ((fx1(x)*x)-(fx(x)-k))/fx1(x);
  }
  return x;
}
