#include "LiquidCrystal_PCF8574.h"

LiquidCrystal_PCF8574 lcd(0x27);

void setupLCD() {
  Wire.begin();
  Wire.beginTransmission(0x27);
  int error = Wire.endTransmission();

  if (DEBUG) {
    Serial.print("Error: "); Serial.println(error);
    if (error == 0) Serial.println(": LCD found.");
    else Serial.println(": LCD not found.");
  }

  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(255);
}

void lcdPrint(char s[]) {
  int len = strlen(s);
  lcd.clear();
  if (len > 16) {
    len = min(len, 32);
    char s1[20], s2[20];
    for (int i = 0; i < 16; i++) s1[i] = s[i]; s1[16] = '\0';
    for (int i = 16; i < len; i++) s2[i-16] = s[i]; s2[len-15] = '\0';
    lcd.setCursor(0, 0); lcd.print(s1);
    lcd.setCursor(0, 1); lcd.print(s2);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(s);
  }
}
