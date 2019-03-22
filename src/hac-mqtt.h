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
	printDebug(debugMessage);

	String topicRef = String(machine_id) + "/command/action";
		if (topicRef == topic){
			if (payload == "0"){
		    activate  = 0;
			}
			else if (payload == "1"){
        lcdPrint("Starting New Session");
			  activate	= 1;
			  startMillis = millis();
			}
			else if (payload == "2"){
				lcdPrint("User Not Certified");
				delay(2500);
				welcomeScreen();
			}
			else if (payload == "3"){
				lcdPrint("Unknown User    Contact Admin");
				delay (2500);
				welcomeScreen(); 
			}
		}
}

void setupMQTT() {
	WiFi.begin(ssid, password);
	client.begin(mqtt_server, mqtt_port, net);
	client.onMessage(messageReceived);
	connect();
}
