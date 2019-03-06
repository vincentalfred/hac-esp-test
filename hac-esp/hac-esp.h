#include <SPI.h>
#include <Wire.h>

#include "MFRC522.h"
#include "LiquidCrystal_PCF8574.h"

#define MFRC522_SS_PIN D4
#define MFRC522_RST_PIN D3

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

MFRC522 rfid(MFRC522_SS_PIN, MFRC522_RST_PIN);
MFRC522::MIFARE_Key key;

void setupLCD() {
  Wire.begin();
  Wire.beginTransmission(0x27);
  int error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);
  
  if (error == 0) {
    Serial.println(": LCD found.");
  
  } else {
    Serial.println(": LCD not found.");
  }
  
  lcd.begin(16, 2); // initialize the lcd  
}

void setupRFID(){
  SPI.begin();
//  pinMode(MFRC522_SS_PIN, OUTPUT);
  rfid.PCD_Init();
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
//  digitalWrite(MFRC522_SS_PIN, HIGH);
//  digitalWrite(MFRC522_SS_PIN, LOW);
}

bool getCardUID(char carduid[]){
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent())
      return false;
  
  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial())
      return false;
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < rfid.uid.size; i++) {
      carduid[i*2] = "0123456789ABCDEF"[rfid.uid.uidByte[i]/16];
      carduid[i*2+1] = "0123456789ABCDEF"[rfid.uid.uidByte[i]%16];
  } carduid[rfid.uid.size*2] = '\0';
  Serial.println(carduid);
  
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  return true;
}
