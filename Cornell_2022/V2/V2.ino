/* INITIAL DATA TESTING SCRIPT */
#include <LiquidCrystal.h>
#include <Filters.h>


#define READPIN A1
const int RS=2, E=3, D4=5, D5=6, D6=7, D7=8; //LCD Pins
const int RED=11, GRN=10, BLU=9; //LED Pins

const int TOGGLE = 12;
/*Weight ranges in GRAMS, R1 <= R2*/

//RED
const double R1_MIN = 30;
const double R1_MAX = 220;

//GREEN
const double R2_MIN = 180;
const double R2_MAX = 700;

//BLUE
const double R3_MIN = 700;
const double R3_MAX = 1000;

double offset = 0; //offset reading for sensor
const int TESTS = 1;
const int INIT_TESTS = 1000;
; //amount of times for the microcontroller to measure data from the sensor. Its all averaged out

FilterOnePole stage0(LOWPASS, 1, 0);
FilterOnePole stage1(LOWPASS, 0.3, 0);
RunningStatistics avg;

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
  lcd.print("DBMAIN_MIT");

  Serial.begin(9600);
}

double gx(double x);
double gx1(double x);
double newtonsmethod(double k);

void loop() {
  double finalsum = 0;
  double mass = 0;
  stage0.input(analogRead(READPIN));
  stage1.input(stage0.output());
  avg.input(stage1.output());
  Serial.println(avg.mean());
  /*
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
      lcd.print(finalsum);
      lcd.print("");
      /*
      if ((mass > R1_MIN) && (mass < R1_MAX)) {
        digitalWrite(RED, HIGH);
        digitalWrite(GRN, LOW);
        digitalWrite(BLU, LOW);
      }
      else if ((mass > R2_MIN) && (mass < R2_MAX)) {
        digitalWrite(RED, LOW);
        digitalWrite(GRN, HIGH);
        digitalWrite(BLU, LOW);
      }
      else if ((mass > R3_MIN) && (mass < R3_MAX)) {
        digitalWrite(RED, LOW);
        digitalWrite(GRN, LOW);
        digitalWrite(BLU, HIGH);
      }
      interrupt = true;
    }
   
  }
  */
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
