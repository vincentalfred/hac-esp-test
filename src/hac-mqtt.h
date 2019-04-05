#include <ESP8266WiFi.h>
#include <MQTT.h>

const char* ssid        = "Android AP";
const char* password    = "abcdefgh";
const char* mqtt_server = "192.168.3.156";
const int   mqtt_port   = 1883;

WiFiClient net;
MQTTClient client;

void connect() {
	printDebug("Checking WiFi...");
	while (WiFi.status() != WL_CONNECTED) {
		printDebug("...");
		delay(1000);
	}
	printDebug("WiFi connected!"); delay(1000);
	while (!client.connect(machine_name)) {
		printDebug("Connecting to MQTT Broker");
		delay(1000);
	}
	printDebug("MQTT connected!"); delay(1000);

	String topic = String(machine_id) + "/command/#";
  	client.subscribe(topic);
	printDebug("sub to " + topic);
}

void connectRaspi() {
	// printDebug("Waiting for Raspi...");
	String topics = String(machine_id) + "/state/connect";
	client.publish(topics, "?");
	raspiResponse = 0;
	raspiMillis = millis();
	if (firstRaspiConnect) firstRaspiConnect = 0;
}

bool raspiConnected() {
	unsigned long curRaspiMillis = millis();
	if (firstRaspiConnect) {
		connectRaspi();
		return false;
	}
	if (!connectedToRaspi) {
		if (curRaspiMillis-raspiMillis > raspiInterval) connectRaspi();
		return false;
	}
	if (connectedToRaspi){
		if (curRaspiMillis-raspiMillis > raspiInterval) {
			if (raspiResponse == 0) {
				connectedToRaspi = 0;
				return false;
			}
			connectRaspi();
		}
		return true;
	}
	return false;
}

void messageReceived(String &topic, String &payload) {
	String debugMessage = "incoming " + payload;
	printDebug(debugMessage);

	String topicRef = String(machine_id) + "/command/action";
	if (topicRef == topic){
		if (payload == "0") {
	    	activate = 0;
		}
		else if (payload == "1"){
			lcdPrint("Starting New Session");
			activate	= 1;
			startMillis = millis();
			minuteMillis = startMillis;
			startEnergy = pzem.energy(ip);
		}
		else if (payload == "2"){
			deniedScreen();
			delay(2500);
			welcomeScreen();
		}
		else if (payload == "3"){
			unknownCardScreen();
			delay (2500);
			welcomeScreen();
		}
	}
	topicRef = String(machine_id) + "/command/connect";
	if (topicRef == topic){
		raspiResponse = 1;
		connectedToRaspi = 1;
	}
}

void setupMQTT() {
	WiFi.begin(ssid, password);
	client.begin(mqtt_server, mqtt_port, net);
	client.onMessage(messageReceived);
	connect();
}
