import paho.mqtt.client as mqtt
from paho.mqtt.client import CallbackAPIVersion
import json
import time
import sys

# --- MQTT CREDENTIALS (should match your ESP8266) ---
MQTT_BROKER = "d2030e1fd8da4f139d685c7323568db6.s1.eu.hivemq.cloud"
MQTT_PORT = 8883
MQTT_USER = "MAJOR_PROJECT"
MQTT_PASS = "Majorproject27567"
MQTT_DATA_TOPIC = "crackmonitor/data"      # Topic for receiving sensor data
MQTT_CONTROL_TOPIC = "crackmonitor/control" # NEW: Topic for sending commands
# ----------------------------------------------------

# The callback for when the client connects.
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # Subscribe to the DATA topic to receive sensor readings
        print(f"Subscribing to data topic: {MQTT_DATA_TOPIC}")
        client.subscribe(MQTT_DATA_TOPIC)
    else:
        print(f"Failed to connect, return code {rc}\n")
        sys.exit() # Exit if connection fails

# The callback for when a data message is received.
def on_message(client, userdata, msg):
    # This function now only handles incoming sensor data
    try:
        payload_str = msg.payload.decode()
        data = json.loads(payload_str)
        
        distance = data.get("distance_cm")
        
        if distance is not None:
            print(f"Received | Distance: {distance} cm")
        else:
            print(f"Received malformed data: {payload_str}")

    except Exception as e:
        print(f"An error occurred while processing message: {e}")

# --- Standard MQTT Client Setup ---
client = mqtt.Client(CallbackAPIVersion.VERSION2)
client.username_pw_set(MQTT_USER, MQTT_PASS)
client.tls_set()

client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
except Exception as e:
    print(f"Could not connect to MQTT Broker: {e}")
    sys.exit()

# Start the network loop in a background thread. This is essential.
client.loop_start()

# --- NEW: Main Control Logic ---
print("Client connected. Ready to send start command.")
while True:
    # Prompt the user for a command
    command = input("Type 'start' to begin monitoring, or 'exit' to quit: ").lower().strip()
    
    if command == "start":
        print(f"Sending 'start' command to topic: {MQTT_CONTROL_TOPIC}")
        # Publish the start command to the control topic
        client.publish(MQTT_CONTROL_TOPIC, "start")
        print("Start command sent. Now listening for sensor data...")
        break # Exit this control loop to only listen for messages
        
    elif command == "exit":
        print("Exiting...")
        client.loop_stop()
        client.disconnect()
        sys.exit()
        
    else:
        print("Invalid command. Please type 'start' or 'exit'.")

# --- Keep script alive to listen for data messages ---
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nExiting...")
    client.loop_stop()
    client.disconnect()