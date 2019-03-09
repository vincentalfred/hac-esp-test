#include <ESP8266WiFi.h>
#include <MQTT.h>

const char* ssid        = "Android AP";
const char* password    = "abcdefgh";
const char* mqtt_server = "192.168.3.157";
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

	// char *topic; sprintf(topic, "%d/command/#", machine_id);
	String topic = String(machine_id) + "/command/#";
  	client.subscribe(topic);
	printDebug("sub to "+ topic);
}

void messageReceived(String &topic, String &payload) {
	String debugMessage = "incoming " + payload;
	Serial.println(debugMessage);
	printDebug(debugMessage);
}

void setupMQTT() {
	WiFi.begin(ssid, password);
	client.begin(mqtt_server, mqtt_port, net);
	client.onMessage(messageReceived);
	connect();
}
