#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <PZEM004T.h>

#define SS_MFRC522 D4  
#define RST_MFRC522 D3
#define SSR D0
#define BUZZER D8

HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
IPAddress ip(192,168,1,1);

MFRC522 rfid(SS_MFRC522, RST_MFRC522);
MFRC522::MIFARE_Key key; 


bool pzemrdy = false;
byte nuidPICC[4];
char uid[12];
char nim[10];
String error_id = "No Matched ID Found";
 

void setup() {
  // put your setup code here, to run once:
  pinMode(SS_MFRC522, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);  
  SPI.begin(); 
  SPI.transfer(0); 
  lcd.begin(16, 2); 
  lcd.setBacklight(255);
  rfid.PCD_Init(); 
  rfid.PCD_SetAntennaGain(rfid.RxGain_max); 
  digitalWrite(SS_MFRC522, HIGH);
  digitalWrite(SS_MFRC522, LOW);
  while (!pzemrdy) {
    Serial1.println("Connecting to PZEM...");
    pzemrdy = pzem.setAddress(ip);
    delay(1000);
  }
  delay(200);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(1, 0);
  lcd.print("Ready to use");    


  if (! rfid.PICC_IsNewCardPresent())
      return; 
  if (! rfid.PICC_ReadCardSerial())
      return; 

  for (byte i = 0; i < 4; i++){
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
  cekUID();
  printhex(rfid.uid.uidByte, rfid.uid.size); 
  clearnama();  
  Serial.print(uid); 
  lcd.clear(); 
  rfid.PICC_HaltA(); 
  rfid.PCD_StopCryptol(); 
}

void cekUID(){
  
  }
void clearnama(){
   for(int a = 0; a<=13; a++){
      nama[a] = 0;
    }  
  }

void printhex(byte *buffer, byte buffersize){
    int n = 0; 
    for byte (i = 0l i < buffersize; i++){
        if(buffer[i] < 16){
            n+=snprintf (&uid[n], 100-n, "0%X", buffer[i]);
           }
        else{
            n+=snprintf (&uid[n], 100-n, "%X", buffer[i]);
           } 
      }
  }
void PowerMeter(){
   float v = pzem.voltage(ip);
  if (v < 0.0) v = 0.0;
   Serial1.print(v);Serial1.print("V; ");

  float i = pzem.current(ip);
   if(i >= 0.0){ Serial1.print(i);Serial1.print("A; "); }

  float p = pzem.power(ip);
   if(p >= 0.0){ Serial1.print(p);Serial1.print("W; "); }

  float e = pzem.energy(ip);
   if(e >= 0.0){ Serial1.print(e);Serial1.print("Wh; "); }

  Serial1.println();

  delay(3000);
  }  
