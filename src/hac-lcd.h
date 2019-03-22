#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);

void setupLCD() {
    Wire.begin();
    Wire.beginTransmission(0x27);
    int error = Wire.endTransmission();

    if (DEBUG && !LCDDEBUG) {
        Serial.print("Error: "); Serial.println(error);
        if (error == 0) Serial.println(": LCD found.");
        else Serial.println(": LCD not found.");
    }

    lcd.begin(16, 2); // initialize the lcd
    lcd.setBacklight(255);
}

void lcdPrint(String s) {
    int len = s.length();
    lcd.clear();
    if (len > 16) {
        len = min(len, 32);
        String s1, s2;
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

void welcomeScreen(){
  lcd.setCursor(0, 0); lcd.print("    Welcome     ");
  lcd.setCursor(0, 1); lcd.print("   I'm Online   ");
}

void endScreen(){
  lcd.setCursor(0, 0); lcd.print(" Session Ended  ");
  lcd.setCursor(0, 1); lcd.print("   Thank You    ");
}

void deniedScreen() {
  lcd.setCursor(0, 0); lcd.print(" Access Denied  ");
  lcd.setCursor(0, 1); lcd.print(" Contact Admin  ");
}

void unknownCardScreen() {
    lcd.setCursor(0, 0); lcd.print("    New Card    ");
    lcd.setCursor(0, 1); lcd.print("    Detected    ");
}

void certifiedScreen(String timeLeft, String powerMeter) {
  lcd.setCursor(0, 0); lcd.print("TimeLeft: " + timeLeft + "Min");
  lcd.setCursor(0, 1); lcd.print("Energy(Wh):" + powerMeter);
}
