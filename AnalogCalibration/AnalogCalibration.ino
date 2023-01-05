/* DB CALIBRATION */
#include <LiquidCrystal.h>
const int RS=2,E=3,D4=8,D5=7,D6=6,D7=5; //LCD pins, 4-bit mode
double offset = 0;
double dvoltage = 0;

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("CALIBRATION MODE");
  lcd.setCursor(0,1);
  lcd.print("ANLOG USE SERIAL");
  Serial.begin(57600);
  calibrate();
  
  
}

void loop() {
  

}

void calibrate() {
  Serial.println("Start calibration");
  Serial.println("Place sensor on level surface");
  Serial.println("Remove any load applied to sensor");
  Serial.println("Send 't' from serial monitor to tare offset");

  boolean wait = true;
  while (wait == true) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') {
        offset = analogRead(A0);
        Serial.println("Tare complete!");
        Serial.print("Offset value: ");
        Serial.println(offset);
        wait = false;
      }
    }
  }
  Serial.println();
  Serial.println("Place known mass on sensor");
  Serial.println("Enter weight of known mass (in grams) from serial monitor");
  
  double mass = 0;
  wait = true;
  while (wait == true) {
    if (Serial.available() > 0) {
      mass = Serial.parseFloat();
      if (mass > 0) {
        Serial.print("Known mass: ");
        Serial.println(mass);
        Serial.println();
        wait = false;
      }
    }
  }
  dvoltage = (analogRead(A0)+analogRead(A1)+analogRead(A2))/3;
  double scale = ((dvoltage-offset)/mass);
  Serial.print("Calibration value (SCALE): ");
  Serial.println(scale);

  Serial.println("Calibration Ended");
  Serial.println((dvoltage-offset)/scale);
  Serial.println("***");
}
