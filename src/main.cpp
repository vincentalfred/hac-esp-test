#include <Arduino.h>
#include "hac-esp.h"

void setup() {
    //Serial.begin(9600);
    setupLCD();
    setupMQTT();
    setupRFID();
    setupPZEM();
}

char cur_carduid[20] = {};
bool activate = 0; // value from cloud to ack SSR pin D8

void loop(){
    client.loop();
    if (!client.connected()) connect();

    char carduid[20];
    if (getCardUID(carduid) && strcmp(carduid, cur_carduid) != 0) {
        strcpy(cur_carduid, carduid);
        printDebug(carduid);
        String topic = String(machine_id) + "/state/carduid";
        client.publish(topic, carduid);

        if (activate == 1 ) digitalWrite(SSR_PIN, HIGH); // to turn on SSR

        if (SSR_PIN == HIGH){
            powerDisplay(); 
        }
  }
