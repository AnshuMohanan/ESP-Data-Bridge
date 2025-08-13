// data_transfer.ino

// --- Core Libraries ---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// --- Custom Modules ---
#include "config.h"
#include "wifi_manager.h"
#include "mqtt_handler.h"

// --- MQTT Configuration DEFINITIONS ---
// This is the one place where these variables are created.
const char* MQTT_SERVER = "***************";
const int MQTT_PORT = ****;
const char* MQTT_USER = "*****************";
const char* MQTT_PASS = "*****************";
const char* MQTT_DATA_TOPIC = "***********";
const char* MQTT_CONTROL_TOPIC = "********";

// --- Initialize Library Objects ---
WiFiClientSecure espClient;
PubSubClient client(espClient);

// --- Global variables ---
unsigned long lastSendTime = 0;
bool monitoring_active = false;

//==================================================================
// THE COMPILER IS LOOKING FOR THIS FUNCTION. IT MUST BE PRESENT.
//==================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Ultrasonic Crack Monitor ---");

  // Initialize hardware pins
  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(MQTT_LED_PIN, OUTPUT);
  digitalWrite(WIFI_LED_PIN, LOW);
  digitalWrite(MQTT_LED_PIN, LOW);

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Initialize Wi-Fi
  initializeWifi();

  // Initialize MQTT
  setupMqtt(espClient, client);
}

//==================================================================
// THE COMPILER IS ALSO LOOKING FOR THIS FUNCTION. IT MUST BE PRESENT.
//==================================================================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFI_LED_PIN, LOW);
    digitalWrite(MQTT_LED_PIN, LOW);
    monitoring_active = false; // Reset state on disconnect
    Serial.println("Wi-Fi Disconnected. Re-initializing...");
    initializeWifi(); // Try to reconnect
  } else {
    digitalWrite(WIFI_LED_PIN, HIGH);
    if (!client.connected()) {
      reconnectMqtt(client);
    }
    client.loop(); // Process incoming MQTT messages

    // Main sensor and data logic
    if (client.connected() && monitoring_active) {
      if (millis() - lastSendTime > SEND_INTERVAL) {
        long distance = readUltrasonicDistance();
        Serial.print("Monitoring Active. Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        String payload = "{\"distance_cm\":" + String(distance) + "}";
        client.publish(MQTT_DATA_TOPIC, payload.c_str());
        lastSendTime = millis();
      }
    } else if (client.connected() && !monitoring_active) {
      // If connected but not active, just wait.
      if (millis() - lastSendTime > 5000) {
        Serial.println("Connected and waiting for 'start' command...");
        lastSendTime = millis();
      }
    }
  }
}

// This function is called by loop()
long readUltrasonicDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

