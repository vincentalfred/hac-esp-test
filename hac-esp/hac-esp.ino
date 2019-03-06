#include "hac-esp.h"

void setup(){
  Serial.begin(9600);
  setupLCD();
  setupRFID();
  lcd.setBacklight(255);
}

void loop(){
  char carduid[20];
  if (getCardUID(carduid)) {
    Serial.print("Got Card UID! = ");
    Serial.println(carduid);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(carduid);
  }
}
