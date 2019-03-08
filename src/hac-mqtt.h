#include <ESP8266WiFi.h>
#include "PubSubClient.h"

const char* ssid        = "Android AP";
const char* password    = "abcdefgh";
const char* mqtt_server = "192.168.3.157";
const int   mqtt_port   = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setupWifi();
void setupMQTT();
void callback(String topic, byte* message, unsigned int length);
void reconnect();

void setupWifi(){
	delay(10);
	// We start by connecting to a WiFi network

	// Serial.print("\nConnecting to "); Serial.println(ssid);
  char debugMessage[50]; sprintf(debugMessage, "connecting to %s", ssid);
	printDebug(debugMessage);

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	printDebug("WiFi connected!");
}

void setupMQTT(){
	setupWifi();
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: "); Serial.print(topic);
  Serial.print(". Message: ");

  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  if(topic=="led" && messageTemp[0] == 'S') {
    Serial.print("Changing GPIO 2 to ");

  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
		printDebug("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      printDebug("MQTT Connected!");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)

			char* topic; sprintf(topic, "%d/command/#", machine_id);
      client.subscribe(topic);
    } else {
			char debugMessage[105];
			sprintf(debugMessage, "failed, rc=%d. Try again in 5s", client.state());
			printDebug(debugMessage);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
