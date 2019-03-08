#include <ESP8266WiFi.h>
#include "PubSubClient.h"

const char* ssid        = "Android AP";
const char* password    = "abcdefgh";
const char* mqtt_server = "192.168.3.155";
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
  char s[50]; sprintf(s, "connecting to %s", ssid);
	if (DEBUG) lcdPrint(s);
	else Serial.println(s);

  lcdPrint(s);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.print("\nWiFi connected - ESP IP address: ");
	Serial.println(WiFi.localIP());
}

void setupMQTT(){
	setupWifi();
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
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
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
