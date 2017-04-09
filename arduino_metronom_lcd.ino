#include "LiquidCrystal.h"

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  lcd.begin(16,2);
  lcd.print("Hello World!");
  
}

void loop() {
  lcd.setCursor(0,1);
  lcd.print(millis() / 1000);

}
