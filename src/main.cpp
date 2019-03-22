#include <Arduino.h>
#include "hac-esp.h"

void setup() {
		if (DEBUG && !LCDDEBUG) Serial.begin(9600);
		setupLCD();
		setupMQTT();
		setupRFID();
		setupPZEM();
		welcomeScreen();
}

char cur_carduid[20] = {};
char carduid[20];


void loop(){
		client.loop();
		if (!client.connected()) connect();

		if (activate == 0) {
			if (getCardUID(carduid)) {
					strcpy(cur_carduid, carduid);
					printDebug(carduid);
					String topic = String(machine_id) + "/state/carduid";
					client.publish(topic, carduid);

					}
		}
		 else if (activate == 1 ){
			digitalWrite(SSR_PIN, HIGH);
			currentMillis = millis();
			int elapseTime = (currentMillis - startMillis)/60000;
			int remainingTime = 30 - elapseTime;
			if (currentMillis - startMillis < interval){
				float e = pzem.energy(ip);
				char buffer4[4];
				String energys = dtostrf(e , 4, 0, buffer4);
				certifiedScreen(String(remainingTime), energys);
				if((currentMillis - startMillis)%60000 == 0){
					String topicEnergy = String(machine_id) + "/state/usage";
					client.publish(topicEnergy, energys);
				}
				if (remainingTime <= 3){
					lcdBlink = 1; lcdMillis = currentMillis;
					if (getCardUID(carduid) && strcmp(carduid, cur_carduid) == 0){
						lcdBlink = 0;
						startMillis = currentMillis;
					}
				}
			}

			int stopButton = digitalRead(D0);
			if (currentMillis - startMillis >= interval || stopButton == 1){
				float e = pzem.energy(ip);
				char buffer4[4];
				String energys = dtostrf(e , 4, 0, buffer4);
				String topicEnergy = String(machine_id) + "/state/stop";
				client.publish(topicEnergy, energys);
				digitalWrite(SSR_PIN, LOW);
				activate = 0;
				endScreen();
			}
			if (lcdBlink == 1 && currentMillis - lcdMillis >= 1000){
				lcdBacklight = 255 - lcdBacklight;
				lcd.setBacklight(lcdBacklight);
				lcdMillis = currentMillis;

			}
		}
}
