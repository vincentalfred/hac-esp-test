#include <ESP8266WiFi.h>
#include <MQTT.h>

const char* ssid        = "Android AP";
const char* password    = "abcdefgh";
const char* mqtt_server = "192.168.3.154";
const int   mqtt_port   = 1883;

WiFiClient net;
MQTTClient client;

void connect() {
	printDebug("Checking WiFi...");
	while (WiFi.status() != WL_CONNECTED) {
		printDebug("...");
		delay(1000);
	}
	printDebug("WiFi connected!");
	while (!client.connect(machine_name)) {
		delay(1000);
	}

	printDebug("MQTT connected!");

	String topic = String(machine_id) + "/command/#";
  	client.subscribe(topic);
	printDebug("sub to " + topic);
}

void messageReceived(String &topic, String &payload) {
	String debugMessage = "incoming " + payload;
	Serial.println(debugMessage);
	printDebug(debugMessage);

	/*
	String command = String(machine_id) + "/command/ssr";
	if (topic == command) {
		if (payload == "0") digitalWrite(SSR_PIN, LOW);
		else {

		}
	}
	command = String(machine_id) + "/command/time";
	if (topic == command) {
		timeLeft = payload;
	}
	*/

}

void setupMQTT() {
	WiFi.begin(ssid, password);
	client.begin(mqtt_server, mqtt_port, net);
	client.onMessage(messageReceived);
	connect();
}
