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
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin();
  Wire.setClock(400000);
  if (nau.begin() == false) {
    lcd.clear();
    lcd.print("NAU7802 NOT FOUND!");
  }
  for (uint8_t i = 0; i<10; i++) {
    while (nau.available() == false) {
      delay(1); 
    }
    nau.getReading();
  }
}

void loop() {
  double value = nau.getAverage(AVG);
  Serial.print("DIG: ");
  Serial.println(value * (3.3/pow(2, 23)), 6);
 
}
 
