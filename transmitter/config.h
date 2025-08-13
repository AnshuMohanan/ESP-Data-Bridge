// config.h

#ifndef CONFIG_H
#define CONFIG_H

// --- Pin and Timing Definitions ---
// These are true constants and are fine as they are.
const int WIFI_LED_PIN = D5;
const int MQTT_LED_PIN = D6;
#define ULTRASONIC_TRIG_PIN D1
#define ULTRASONIC_ECHO_PIN D2
const int MAX_WIFI_CREDENTIALS = 5;
const int SSID_MAX_LEN = 32;
const int PASS_MAX_LEN = 64;
const long SEND_INTERVAL = 2000;

// --- MQTT Configuration Declarations ---
// Use 'extern' to DECLARE that these variables exist somewhere else.
// This does NOT create the variable, it just announces it.
extern const char* MQTT_SERVER;
extern const int MQTT_PORT;
extern const char* MQTT_USER;
extern const char* MQTT_PASS;
extern const char* MQTT_DATA_TOPIC;
extern const char* MQTT_CONTROL_TOPIC;

#endif
