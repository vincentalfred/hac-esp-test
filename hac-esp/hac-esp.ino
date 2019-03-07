 #include "hac-esp.h"

void setup(){
  Serial.begin(9600);
  setupLCD();
  setupMQTT();
  setupRFID();
}

char cur_carduid[20] = {};

void loop(){
  char carduid[20];
  if (getCardUID(carduid) && strcmp(carduid, cur_carduid) != 0) {
    strcpy(cur_carduid, carduid);
    Serial.print("Got Card UID! = ");
    Serial.println(carduid);
    Serial.print("cur = "); Serial.println(cur_carduid);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(carduid);
  }

  if (!client.connected()) reconnect();
  if(!client.loop()) client.connect("ESP8266Client");

  
}
