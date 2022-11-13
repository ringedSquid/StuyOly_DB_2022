#include <LiquidCrystal.h>
#define TESTS 5
const int RS=2, E=3, D4=8, D5=7, D6=6, D7=5; //LCD pins
const double offset = 0;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setup() {
  Serial.begin(57600);
}

void loop() {
  double results[TESTS];
  bool wait = true;
  Serial.println("send 't' to measure");
  while (wait == true) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') {
        for (int i=0; i<TESTS; i++) {
          results[i] = avg_measure(500);
          if ((i % 5) == 0) {
            Serial.print("*");
          }
        }
        Serial.println();
        Serial.println("Results: ");
        double finalsum = 0;
        for (int i=0; i<TESTS; i++) {
          //Serial.println(results[i]);
          finalsum += results[i];
        }
        Serial.print("AVERAGE: ");
        Serial.println(finalsum/TESTS);
        Serial.print("ESTIMATED MASS: ");
        Serial.print(solve(finalsum/TESTS));
        Serial.println("g");
        Serial.print("DERIVATIVE:");
        Serial.println(fx1(finalsum/TESTS));
        
        wait = false;
      }
    }
  }
}

double avg_measure(int times) {
  double finalsum = 0;
  for (int i=0; i<times; i++) {
    finalsum += (analogRead(A0)+analogRead(A1)+analogRead(A2))/3;
  }
  return finalsum/times;
}

/* Solving polynomial of degree 6
 *  202 + 0.0673x + (1.61*10^-3x^2) + (-4.79*10^-6x^3) + (4.99*10^-9x^4) + (-1.75*10^-12x^5) + (-2.97*10^-17x^6)
 *  DERIVATIVE
 *  0.0673 + 0.00322 x - 0.00001437 x^2 + 1.996*10^-8 x^3 - 8.75*10^-12 x^4 - 1.7820000000000004*10^-16 x^5
*/

double fx(double x) { //og equation
  return (202) + (0.0673*x) + (1.61*pow(10,-3)*pow(x,2)) + (-4.79*pow(10,-6)*pow(x,3)) + (4.99*pow(10,-9)*pow(x,4)) + (-1.75*pow(10,-12)*pow(x,5)) + (-2.97*pow(10,-17)*pow(x,6));
}

double fx1(double x) { //derivative
  return (0.0673) + (0.00322*x) - (0.00001437*pow(x,2)) + (1.996*pow(10,-8)*pow(x,3)) - (8.75*pow(10,-12)*pow(x,4)) - (1.782*pow(10,-16)*pow(x,5));
}

double solve(double k) { 
  double x = 100;
  for (int i=0; i<30; i++) {
    x = ((fx1(x)*x)-(fx(x)-k))/fx1(x);
  }
  return x;
}
