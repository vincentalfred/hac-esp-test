#include <Arduino.h>
#include "hac-esp.h"

void setup() {
    Serial.begin(9600);
    setupLCD();
    setupMQTT();
    setupRFID();
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

    // print to LCD time and usage
}
