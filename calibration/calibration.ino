/* DB CALIBRATION */
#include <LiquidCrystal.h>
#include <HX711.h>
const int RS=2,E=3,D4=8,D5=7,D6=6,D7=5; //LCD pins, 4-bit mode
const int DT=12,LD_SCK=13; //HX711 pins

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);
HX711 sensor;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("CALIBRATION MODE");
  lcd.setCursor(0,1);
  lcd.print("HX711 USE SERIAL");
  sensor.begin(DT,SCK);
  Serial.begin(57600);
  while (!(sensor.is_ready())) {
    Serial.println("Waiting for sensor...");
    delay(500);
  }
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
        sensor.tare(10);
        Serial.println("Tare complete!");
        twait = false;
      }
    }
  }
  Serial.println();
  Serial.println("Place known mass on sensor");
  Serial.println("Enter weight of known mass (in grams) from serial monitor");
  
  float mass = 0;
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

  float scale = (sensor.get_units(20)/mass);
  Serial.print("Calibration value (SCALE): ");
  Serial.println(scale);

  Serial.println("Calibration Ended");
  Serial.println("***");
}
