/*
  Use the Qwiic Scale to read load cells and scales
  By: Nathan Seidle @ SparkFun Electronics
  Date: March 3rd, 2019
  License: This code is public domain but you buy me a beer if you use this 
  and we meet someday (Beerware license).

  The Qwiic Scale is an I2C device that converts analog signals to a 24-bit
  digital signal. This makes it possible to create your own digital scale
  either by hacking an off-the-shelf bathroom scale or by creating your
  own scale using a load cell.

  This example merely outputs the raw data from a load cell. For example, the
  output may be 25776 and change to 43122 when a cup of tea is set on the scale.
  These values are unitless - they are not grams or ounces. Instead, it is a
  linear relationship that must be calculated. Remeber y = mx + b?
  If 25776 is the 'zero' or tare state, and 43122 when I put 15.2oz of tea on the
  scale, then what is a reading of 57683 in oz?

  (43122 - 25776) = 17346/15.2 = 1141.2 per oz
  (57683 - 25776) = 31907/1141.2 = 27.96oz is on the scale
  
  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/15242

  Hardware Connections:
  Plug a Qwiic cable into the Qwiic Scale and a RedBoard Qwiic
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 9600 baud to see the output
*/

#include <Wire.h>
#include <Filters.h>
#include <LiquidCrystal.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU7802

//LCD PINS
const int RS = 2, E = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7; 
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
NAU7802 myScale; //Create instance of the NAU7802 class
FilterOnePole lowpiss(LOWPASS, 10, 0);
FilterOnePole lowpiss2(LOWPASS, 1, 0);
FilterOnePole lowpiss3(LOWPASS, 0.1, 0);
RunningStatistics lowstats;

void setup()
{
  Serial.begin(19200);

  Wire.begin();
  lcd.begin(16, 2);
  lcd.clear();

  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  myScale.setGain(NAU7802_GAIN_128);
  myScale.setLDO(NAU7802_LDO_4V5);
  myScale.setRegister(NAU7802_ADC, 0x30);
  myScale.setBit(NAU7802_PGA_PWR_PGA_CAP_EN, NAU7802_PGA_PWR);
  myScale.setSampleRate(NAU7802_SPS_80);
  myScale.calibrateAFE();
}
int cycle = 500000;
void loop()
{
  if(myScale.available() == true)
  {
    long avgRead = 0;
    for (int i = 0; i< 1; i++) {
      long currentReading = myScale.getReading();
      lowpiss.input(currentReading);
      lowpiss2.input(lowpiss.output());
      lowstats.input(lowpiss2.output());
    }
    Serial.println((lowstats.mean() + 456375)/20);
  }
}
