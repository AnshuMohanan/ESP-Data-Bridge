# 📡 ESP8266 MQTT Data Bridge

[![Arduino IDE](https://img.shields.io/badge/Arduino-ESP8266-00979D?style=for-the-badge&logo=arduino)](https://www.arduino.cc/)
[![Python](https://img.shields.io/badge/Python-3.10+-3776AB?style=for-the-badge&logo=python)](https://www.python.org/)
[![MQTT](https://img.shields.io/badge/MQTT-Broker-660066?style=for-the-badge&logo=mqtt)](https://mqtt.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A robust framework for the ESP8266 to wirelessly transmit any sensor data over Wi-Fi using the MQTT protocol. This project features a smart Wi-Fi manager for seamless connectivity and a modular, command-controlled architecture, making it a perfect foundation for any IoT data-logging and remote monitoring application.

---

## Table of Contents

- [About The Project](#About-The-Project)
- [Key Features](#Key-Features)
- [System Architecture](#System-Architecture)
- [Hardware and Software](#Hardware-and-Software)
- [Getting Started](#Getting-Started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Mode 1: Batch Processing](#mode-1-batch-processing)
  - [Mode 2: Real-Time Simulation](#mode-2-real-time-simulation)
- [Technologies Used](#technologies-used)
- [License](#license)
- [Authors](#authors)
- [Acknowledgments](#acknowledgments)
---

## About The Project

This project provides a reliable and modular solution for collecting data from any sensor connected to an ESP8266 and transmitting it wirelessly to a central receiver. It moves beyond simple scripts by implementing a robust system for handling Wi-Fi connections and MQTT communication, which can be easily adapted for various sensors (e.g., ultrasonic, temperature, humidity, etc.).

The core of the project is its two-way communication system. The ESP8266 node waits for a `start` command from a central client before it begins collecting and sending data. This command-and-control structure prevents unnecessary data transmission and allows for remote management of the sensor node.

---

## Key Features

- **🔌 Smart Wi-Fi Manager**: Automatically scans for and connects to known Wi-Fi networks stored in EEPROM. If no known networks are found, it provides a simple serial interface to add new credentials.
- **🚀 Reliable MQTT Communication**: Uses the secure and lightweight MQTT protocol to publish sensor data and subscribe to control commands, ensuring efficient and low-latency data transfer.
- **💻 Remote Command Control**: The ESP8266 node remains idle until it receives a "start" command, allowing for on-demand data collection and reducing network traffic.
- **🧱 Modular Codebase**: The C++ code is neatly organized into separate modules for Wi-Fi management (`wifi_manager`), MQTT logic (`mqtt_handler`), and main application flow, making it easy to read, maintain, and expand.
- **⚙️ Sensor Agnostic**: While the example uses an ultrasonic sensor, the `data_transfer.ino` file can be easily modified to read from any type of sensor and publish its data in a structured JSON format.
- **💡 Status LEDs**: Provides visual feedback for Wi-Fi and MQTT connection statuses.

---

## System Architecture

The system operates in a simple yet effective client-server model using an MQTT broker as the intermediary.

1.  **ESP8266 Node**:
    -   Powers on and initializes its Wi-Fi manager.
    -   Connects to a known Wi-Fi network.
    -   Connects to the HiveMQ MQTT broker and subscribes to the `crackmonitor/control` topic.
    -   Enters an idle state, waiting for a command.

2.  **Python Receiver Client**:
    -   Connects to the same MQTT broker and subscribes to the `crackmonitor/data` topic.
    -   The user types `start` in the console.
    -   The client publishes the "start" message to the `crackmonitor/control` topic.

3.  **Data Transfer**:
    -   The ESP8266 receives the "start" message, activates its monitoring loop, and begins reading sensor data.
    -   It formats the data into a JSON payload (e.g., `{"distance_cm": 25}`) and publishes it to the `crackmonitor/data` topic.
    -   The Python client receives the JSON data and prints it to the console.


---

## Hardware and Software

### Hardware
-   An ESP8266-based board (e.g., NodeMCU, Wemos D1 Mini)
-   Any sensor you wish to use (the example uses an HC-SR04 Ultrasonic Sensor)
-   Breadboard and jumper wires

### Software & Libraries
-   **ESP8266/Arduino:**
    -   [Arduino IDE](https://www.arduino.cc/en/software) with the ESP8266 board manager installed.
    -   `PubSubClient` library by Nick O'Leary
    -   `ESP8266WiFi` library (included with the ESP8266 core)
-   **Python Receiver:**
    -   [Python](https://www.python.org/downloads/) (3.8 or newer)
    -   `paho-mqtt` library

---

## Getting Started

### ESP8266 Setup

1.  **Configure the Code**: Open `data_transfer.ino` in the Arduino IDE. All MQTT credentials and topics are already defined in this file. You can modify them here if you are using a different broker.
2.  **Install Libraries**: In the Arduino IDE, go to **Sketch > Include Library > Manage Libraries...** and install `PubSubClient`.
3.  **Upload the Code**: Connect your ESP8266 board, select the correct board and COM port, and upload the `data_transfer.ino` sketch.
4.  **Connect to Wi-Fi**: Open the Serial Monitor (baud rate 115200). The ESP8266 will prompt you to enter the SSID and password for your Wi-Fi network. It will save these credentials to its memory.

### Python Receiver Setup

1.  **Clone the repository (if you haven't already):**
    ```sh
    git clone [https://github.com/your-username/your-project-name.git](https://github.com/your-username/your-project-name.git)
    cd your-project-name
    ```
2.  **Install the MQTT library:**
    ```sh
    pip install paho-mqtt
    ```
3.  **Check Credentials**: Ensure the MQTT credentials in `final_receiver_ultrasonic.py` match those in the ESP8266 code.

---

## Usage

1.  Power on your ESP8266. It should automatically connect to your Wi-Fi and the MQTT broker. The on-board LEDs will indicate the connection status.
2.  Run the Python receiver script from your terminal:
    ```sh
    python final_receiver_ultrasonic.py
    ```
3.  The script will connect to the broker and prompt you for a command. Type `start` and press Enter.
4.  The ESP8266 will receive the command and begin publishing sensor data. You will see the data appear in your Python console in real-time.

---

## Code Structure

The project is organized into several files for clarity and modularity:

-   `data_transfer.ino`: The main entry point for the ESP8266. It contains the `setup()` and `loop()` functions and orchestrates calls to the other modules.
-   `config.h`: A central place for defining hardware pinouts, MQTT topics, and other constants.
-   `wifi_manager.h` / `.cpp`: Handles all Wi-Fi connection logic, including scanning, connecting, and managing credentials.
-   `mqtt_handler.h` / `.cpp`: Manages the connection to the MQTT broker, handles subscriptions, and defines the callback function for incoming messages.
-   `final_receiver_ultrasonic.py`: The Python script that acts as the central monitoring station. It sends control commands and receives sensor data.

---

## License

This project is distributed under the MIT License. See the `LICENSE` file for more information.
