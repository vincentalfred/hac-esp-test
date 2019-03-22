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
        currentMillis = millis();
        int elapseTime = (currentMillis - startMillis)/60000;
        if (currentMillis - startMillis <= interval){
          float e = pzem.energy(ip);
          char buffer4[4];
          String energys = dtostrf(e , 4, 0, buffer4);
          certifiedScreen(String(elapseTime), energys);
          if((currentMillis - startMillis)%60000 == 0){
            String topicEnergy = String(machine_id) + "/state/usage";
            client.publish(topicEnergy, energys);
          }
        }
   else if (activate == 0 || elapseTime == interval){
        float e = pzem.energy(ip);
        char buffer4[4];
        String energys = dtostrf(e , 4, 0, buffer4);
        String topicEnergy = String(machine_id) + "/state/stop";
        client.publish(topicEnergy, energys);
        digitalWrite(SSR_PIN, LOW);
        endScreen();
        }
      }
}
