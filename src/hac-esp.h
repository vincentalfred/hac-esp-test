#include <SPI.h>
#include <Wire.h>

const int machine_id = 1;
const bool DEBUG = true;

#define MFRC522_SS_PIN  D4
#define MFRC522_RST_PIN D3
#define SSR_PIN         D8

#include "MFRC522.h"
#include "hac-lcd.h"
#include "hac-mqtt.h"

MFRC522 rfid(MFRC522_SS_PIN, MFRC522_RST_PIN);
MFRC522::MIFARE_Key key;

void setupRFID(){
  SPI.begin();
  pinMode(MFRC522_SS_PIN, OUTPUT);
  rfid.PCD_Init();
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
  digitalWrite(MFRC522_SS_PIN, HIGH);
  digitalWrite(MFRC522_SS_PIN, LOW);
}

bool getCardUID(char carduid[]){
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent())
      return false;
  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial())
      return false;

  // Show some details of the PICC (that is: the tag/card)
  for (byte i = 0; i < rfid.uid.size; i++) {
      carduid[i*2] = "0123456789ABCDEF"[rfid.uid.uidByte[i]/16];
      carduid[i*2+1] = "0123456789ABCDEF"[rfid.uid.uidByte[i]%16];
  } carduid[rfid.uid.size*2] = '\0';

//  Serial.print(F("PICC type: "));
//  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial.println(rfid.PICC_GetTypeName(piccType));
  return true;
}
