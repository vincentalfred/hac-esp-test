#include <Arduino.h>
#include "hac-esp.h"

void setup() {
    //Serial.begin(9600);
    setupLCD();
    setupMQTT();
    setupRFID();
    setupPZEM();
    welcomeScreen(); 
}

char cur_carduid[20] = {};


void loop(){
    client.loop();
    if (!client.connected()) connect();

    char carduid[20];
    if (getCardUID(carduid) && strcmp(carduid, cur_carduid) != 0) {
        strcpy(cur_carduid, carduid);
        printDebug(carduid);
        String topic = String(machine_id) + "/state/carduid";
        client.publish(topic, carduid);
        }

   if (activate == 1 ){
        digitalWrite(SSR_PIN, HIGH);
        float e = pzem.energy(ip);
        if(e >= 0.0){ Serial.print(e);Serial.print("Wh; "); }
        char buffer4[4];
        String energys = dtostrf(e , 4, 0, buffer4);
        lcd.setCursor(8, 1); lcd.print("Wh:"+energys);
        String topicEnergy = String(machine_id) + "/state/carduid";
        client.publish(topicEnergy, energys);
        certifiedScreen(carduid, energys);
      }

}
