#include <Wire.h>
#include <Filters.h>
#include <LiquidCrystal.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"

/*--WIRING--*/

//LCD PINS
const int RS = 2, E = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7; 

//LED PINS
const int RED = 9, GRN = 10, BLU = 11; 

//PUSHBUTTONS
const int TOGGLE_TAR = 13, TOGGLE_READ = 12; 

/*--RANGES--*/

//Range 1: RED LED
const double R1_MIN = 50;
const double R1_MAX = 71;

//Range 2: GREEN LED
const double R2_MIN = 400;
const double R2_MAX= 501;

//Range 3: BLUE LED
const double R3_MIN = 600;
const double R3_MAX = 800;

/*--DATA CONSTANTS--*/

//SAMPLE COUNT
const int N_AVG = 1000;

//FILTER CUTTOFF
const double CUTOFF = 0.05;

//CALIBRATION FACTOR
//SLOPE
const double CAL_M = 7.26;
//CONSTANT
const double CAL_K = 87109;

//CALIBRATION INIT TIME
const int CAL_CYCL = 5000;

//FLUSH CYCLES
const int FLUSH = 2500;

//ZERO VAL
const long ZERO = 87443;

/*--GLOBALS--*/
long offset= 0;


LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
FilterTwoPole dataFilter(CUTOFF, 1, 0);
NAU7802 nau;

void setup() {
  //init pins
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(TOGGLE_TAR, INPUT);
  pinMode(TOGGLE_READ, INPUT);//Might disable to just have the reading constantly dispalyed

  //init LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("INIT...");

  //start i2c interface
  Wire.begin();

  //init NAU7802
  if (nau.begin() == false) {
    lcd.clear();
    lcd.print("ERR: 0");
    while (1);
  }
  nau.setGain(NAU7802_GAIN_32);
  nau.setLDO(NAU7802_LDO_4V2);
  nau.setRegister(NAU7802_ADC, 0x30);
  //nau.setBit(NAU7802_PGA_PWR_PGA_CAP_EN, NAU7802_PGA_PWR);
  nau.setSampleRate(NAU7802_SPS_320);
  nau.calibrateAFE();

  for (int i = 0; i < CAL_CYCL; i++) {
    dataFilter.input(nau.getReading());
  }
  lcd.clear();
  lcd.print("READY! V3.1");
}

void loop() {
  long data;
  dataFilter.input(nau.getReading());
  if (digitalRead(TOGGLE_READ) == HIGH) {
    lcd.clear();
    lcd.print("MEASURING...");
    if (nau.available() == true) {
      long avgData = 0;
      for (int i = 0; i < FLUSH; i++) {
        dataFilter.input(nau.getReading());
      }
      for (int i = 0; i < N_AVG; i++) {
        dataFilter.input(nau.getReading());
        avgData += dataFilter.output();
      }
      data = (avgData / N_AVG) - offset;
  
      //displaying mass on top row of lcd
      lcd.clear();
      lcd.print("Mass: ");
      double mass = dig_to_mass(data);
  
      //Just for nice leading zeros
      if (mass < 10) { lcd.print("000"); }
      else if (mass < 100) { lcd.print("00"); }
      else if (mass < 1000) { lcd.print("0"); }
  
      lcd.print(mass, 2);
      lcd.print(" g");
  
      //displaying voltage on the second row
      lcd.setCursor(0, 1);
      lcd.print("Voltage: ");
      double voltage = data * (4.2 / 16777216);
      lcd.print(voltage, 2);
      lcd.print(" v");
  
      //LEDS
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
    }
  }

  //TAR PROCEDURE
  if (digitalRead(TOGGLE_TAR) == HIGH) {
    lcd.clear();
    lcd.print("TARE...");
    long avgData = 0;
      for (int i = 0; i < FLUSH; i++) {
        dataFilter.input(nau.getReading());
      }
      for (int i = 0; i < N_AVG; i++) {
        dataFilter.input(nau.getReading());
        avgData += dataFilter.output();
      }
    avgData /= N_AVG;
    offset = avgData - ZERO;
    lcd.clear();
    lcd.print("TARE COMPLETE!");
  }

}

double dig_to_mass(long data) {
  return (data - CAL_K)/CAL_M;
};
