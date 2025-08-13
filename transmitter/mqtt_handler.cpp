// mqtt_handler.cpp

#include <Arduino.h>         // Use Arduino.h for core functions
#include "mqtt_handler.h"
#include "config.h"

// --- MQTT Setup and Reconnect Logic ---

void setupMqtt(WiFiClientSecure& secureClient, PubSubClient& client) {
  secureClient.setInsecure();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback); // This line needs the function below to exist
}

void reconnectMqtt(PubSubClient& client) {
  digitalWrite(MQTT_LED_PIN, LOW);
  monitoring_active = false;
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-Ultrasonic-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected!");
      digitalWrite(MQTT_LED_PIN, HIGH);
      client.subscribe(MQTT_CONTROL_TOPIC);
      Serial.print("Subscribed to control topic: ");
      Serial.println(MQTT_CONTROL_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// --- MQTT Message Handling DEFINITION ---
// The linker was looking for this function. Here it is.
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  if (strcmp(topic, MQTT_CONTROL_TOPIC) == 0) {
    if (strcmp(message, "start") == 0) {
      Serial.println("Start command received! Activating monitoring.");
      monitoring_active = true;
      lastSendTime = millis();
    }
  }
}
