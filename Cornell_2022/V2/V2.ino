/* INITIAL DATA TESTING SCRIPT */
#include <LiquidCrystal.h>
#define READPIN A1
const int RS=2, E=3, D4=5, D5=6, D6=7, D7=8; //LCD Pins
const int RED=11, GRN=10, BLU=9; //LED Pins
const int TOGGLE = 12;
/*Weight ranges in GRAMS, R1 <= R2*/
const double R1 = 415;
const double R2 = 715;
double offset = 0; //offset reading for sensor
const int TESTS = 5000;
const int INIT_TESTS = 1000;
; //amount of times for the microcontroller to measure data from the sensor. Its all averaged out

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setup() {
  double init_avg = 0;
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLU, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GRN, LOW);
  digitalWrite(BLU, LOW);
  pinMode(TOGGLE, INPUT);
  lcd.begin(16, 12);
  lcd.setCursor(0, 0);
  lcd.print("INIT...");
  delay(1000);
  for (int i=0; i<INIT_TESTS; i++) {
    init_avg += analogRead(READPIN);
  }
  offset = init_avg/INIT_TESTS;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DBMAIN_CORN");
}

double gx(double x);
double gx1(double x);
double newtonsmethod(double k);

void loop() {
  double finalsum = 0;
  double mass = 0;
  boolean interrupt = false; //button pressed?
  while (interrupt == false) {
    if (digitalRead(TOGGLE) == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Measuring...");
      lcd.setCursor(0, 1);
      delay(1000);
      for (int i=0; i<TESTS; i++) {
        finalsum += analogRead(READPIN) - offset;
      }
      finalsum /= TESTS;
      mass = newtonsmethod(finalsum);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(finalsum*(5/1023.0));
      lcd.print("v");
      lcd.setCursor(0, 1);
      lcd.print(mass);
      lcd.print("g");
      if (mass < R1) {
        digitalWrite(RED, HIGH);
        digitalWrite(GRN, LOW);
        digitalWrite(BLU, LOW);
      }
      else if ((mass > R1) && (mass < R2)) {
        digitalWrite(RED, LOW);
        digitalWrite(GRN, HIGH);
        digitalWrite(BLU, LOW);
      }
      else {
        digitalWrite(RED, LOW);
        digitalWrite(GRN, LOW);
        digitalWrite(BLU, HIGH);
      }
      interrupt = true;
    }
  }
}


double gx(double x) { //g(x) 
  return (1.16*pow(10, -16)*pow(x, 6)) - (8.71*pow(10,-14)*pow(x,5))-(1.81*pow(10,-10)*pow(x,4)) + (2.79*pow(10,-7)*pow(x,3))-(0.000133*pow(x,2))+(0.221*x)-1.54;
}

double gx1(double x) { //Derivative of g(x)
 return (6.96*pow(10, -16)*pow(x , 5)) -(4.355*pow(10, -13)*pow(x , 4)) - (7.24*pow(10, -10)*pow(x,3))+(8.37*pow(10,-7)*pow(x , 2)) - (0.000266*x) + 0.221;
}
double newtonsmethod(double k) {
  double x = 100;
  for(int i=0; i<30; i++) {
    x = ((gx1(x)*x)-(gx(x)-k))/gx1(x);
  }
  return x;
}
