#include <LiquidCrystal.h>
#include <SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h>
#include <Wire.h>
const int RS = 2, E = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7; //LCD PINS
const int RED = 9, GRN = 10, BLU = 11; //LED PINS
const int TOGGLE_TAR = 13, TOGGLE_READ = 12; //BUTTONS
const int AVG = 100; //Amount of samples taken per reading
double offset = 0;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
NAU7802 nau;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(TOGGLE_TAR, INPUT);
  pinMode(TOGGLE_READ, INPUT);
  Wire.begin();
  Wire.setClock(400000);
  lcd.begin(16, 2);
  lcd.print("INIT...");
  if (nau.begin() == false) {
    lcd.clear();
    lcd.print("NAU7802 NOT FOUND!");
  }
  nau.powerUp();
  nau.setLDO(NAU7802_LDO_4V5);
  nau.setGain(NAU7802_GAIN_128);
  nau.setSampleRate(NAU7802_SPS_320);
  for (uint8_t i = 0; i<10; i++) {
    while (nau.available() == false) {
      delay(1); 
    }
    nau.getReading();
  }
  nau.calibrateAFE();
  lcd.clear();
}

void loop() {
  boolean interrupt = false; //button pressed?
  digitalWrite(RED, HIGH);
  double value = nau.getAverage(AVG);
  while (interrupt == false) {
    if (digitalRead(TOGGLE_READ) == HIGH) {
      lcd.clear();
      lcd.print("DIG: ");
      lcd.print(value);
      lcd.setCursor(0, 1);
      lcd.print(value * (5/16777215.0), 6);
      interrupt = true;
    }
  }
  digitalWrite(RED, LOW);
}
 
