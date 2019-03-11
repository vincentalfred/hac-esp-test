#include <Arduino.h>
#include "hac-esp.h"

void setup() {
    Serial.begin(9600);
    setupLCD();
    setupMQTT();
    setupRFID();
    setupPZEM();
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

    if (SSR_PIN == HIGH){
        float v = pzem.voltage(ip);
        if (v < 0.0) v = 0.0;
         Serial1.print(v);Serial1.print("V; ");

        float i = pzem.current(ip);
         if(i >= 0.0){ Serial1.print(i);Serial1.print("A; "); }

        float p = pzem.power(ip);
         if(p >= 0.0){ Serial1.print(p);Serial1.print("W; "); }

        float e = pzem.energy(ip);
         if(e >= 0.0){ Serial1.print(e);Serial1.print("Wh; "); }
        // String topic = String(pzem.energy(ip)) + "/state/energy";
        // client.publish(topic, pzem.energy(ip));
        Serial1.println();


      }
    delay(1000);
}
