// mqtt_handler.h

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h> // <-- Add this include

// Extern says that this variable exists in another file (the main .ino)
extern bool monitoring_active;
extern unsigned long lastSendTime;

// Update the function to accept WiFiClientSecure
void setupMqtt(WiFiClientSecure& secureClient, PubSubClient& client); 
void mqttCallback(char* topic, byte* payload, unsigned int length);
void reconnectMqtt(PubSubClient& client);

#endif
