/* DB */
#include <LiquidCrystal.h>
#include <HX711.h>

/* LCD pins, 4-bit mode */
const int RS=2,E=3,D4=8,D5=7,D6=6,D7=5;
/* LED pins */
const int RED=11,GRN=10,BLU=9;
/* HX711 amplifier pins */
const int DT=12,LD_SCK=13;
/* HX711 adjustment settings */

const long OFFSET=340;
const long DIVIDER=4730.38;

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);
HX711 sensor;

void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GRN,OUTPUT);
  pinMode(BLU,OUTPUT);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  sensor.begin(DT,LD_SCK);
  sensor.set_scale(DIVIDER);
  sensor.set_offset(OFFSET);
  sensor.tare();
}

void loop() {
  float weight = sensor.get_units(10);
  lcd.print(weight);
  lcd.print("g");
  lcd.setCursor(0,0);
  delay(250);
}
