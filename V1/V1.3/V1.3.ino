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
const int INIT_TESTS = 1000;
; //amount of times for the microcontroller to measure data from the sensor. Its all averaged out

double data, mass;

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
    stage0.input(analogRead(READPIN));
    stage1.input(stage0.output());
    avg.input(stage1.output());
  }
  zero(avg.mean());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V1.3");

  Serial.begin(9600);
}

void loop() {
  stage0.input(analogRead(READPIN));
  stage1.input(stage0.output());
  avg.input(stage1.output());
  data = avg.mean();
  mass = dig_to_mass(data);
  
  boolean interrupt = false; //button pressed?
  while (interrupt == false) {
    if (digitalRead(TOGGLE) == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(data*(5/1023.0));
      lcd.print(" v");
      lcd.setCursor(0, 1);
      lcd.print(mass);
      lcd.print(" g");
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
}

double dig_to_mass(double data) {
  return (data - 222)/0.214;
}

void zero(double data) {
  offset = data;
}
