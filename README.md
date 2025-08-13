# 📡 ESP Data Bridge

[![Arduino IDE](https://img.shields.io/badge/Arduino-ESP8266-00979D?style=for-the-badge&logo=arduino)](https://www.arduino.cc/) [![Python](https://img.shields.io/badge/Python-3.10+-3776AB?style=for-the-badge&logo=python)](https://www.python.org/) [![MQTT](https://img.shields.io/badge/MQTT-Broker-660066?style=for-the-badge&logo=mqtt)](https://mqtt.org/) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](https://opensource.org/licenses/MIT)

A robust framework for the ESP8266 to wirelessly transmit any sensor data over Wi-Fi using the MQTT protocol. This project features a smart Wi-Fi manager for seamless connectivity and a modular, command-controlled architecture, making it a perfect foundation for any IoT data-logging and remote monitoring application.

---

## Table of Contents

- [About The Project](#about-the-project)
- [Key Features](#key-features)
- [System Architecture](#system-architecture)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Hardware Setup & Circuit](#hardware-setup--circuit)
  - [Installation & Setup](#installation--setup)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [License](#license)

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

## Getting Started

Follow these steps to set up the MQTT broker, the ESP8266 transmitter, and the Python receiver.

### Prerequisites

#### Hardware
* An ESP8266-based board (e.g., NodeMCU, Wemos D1 Mini)
* Any sensor you wish to connect

#### Software
* [Arduino IDE](https://www.arduino.cc/en/software) with the ESP8266 board manager installed.
* **Arduino Libraries**:
    * `PubSubClient` (can be installed via the Library Manager)
* [Python](https://www.python.org/downloads/) (3.8 or newer)
* **Python Packages**:
    * `paho-mqtt`

---

## Hardware Setup & Circuit

Before running the code, you need to wire the components correctly. The example uses an HC-SR04 ultrasonic sensor, but you can adapt the connections for your specific sensor.

**Connect your components as shown in the table below:**

| Pin (Sensor) | Pin (ESP8266) | Purpose       |
|--------------|---------------|---------------|
| VCC          | Vin (5V)      | Power         |
| GND          | GND           | Ground        |
| Trig         | D1 (GPIO 5)   | Trigger Pulse |
| Echo         | D2 (GPIO 4)   | Echo Return   |

*Note: Double-check your pin numbers in the circuit diagram located in the demonstration folder .*

---
  
### Installation & Setup

1. Clone the Repository
```sh
git clone [https://github.com/AnshuMohanan/ESP-Data-Bridge.git](https://github.com/AnshuMohanan/ESP-Data-Bridge.git)
cd ESP-Data-Bridge
```

#### Set Up Your MQTT Broker

Before you can run the code, you need a free MQTT broker to handle messages.

1.  **Create an Account**: Go to [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/) and sign up.
2.  **Create a Cluster**: Follow their instructions to create a new, free cluster.
3.  **Get Your Credentials**: In your cluster's "Access Management" or "Credentials" section, get the following:
    * **Server URL** (e.g., `your-id.s1.eu.hivemq.cloud`)
    * **Port** (e.g., `8883`)
    * **Username**
    * **Password**

#### ESP8266 Setup

1.  **Update Credentials**: Open `transmitter/data_transfer.ino` in the Arduino IDE. Replace the placeholder values with **your own HiveMQ credentials** from Step 1.
2.  **Install Libraries**: In the Arduino IDE, go to **Sketch > Include Library > Manage Libraries...** and install `PubSubClient`.
3.  **Upload the Code**: Connect your ESP8266, select the correct board and COM port, and upload the sketch.
4.  **Connect to Wi-Fi**: Open the Serial Monitor (baud rate 115200). The ESP8266 will prompt you to enter the SSID and password for your Wi-Fi network.

#### Python Receiver Setup 

1.  **Navigate to the Receiver Directory:**
    ```sh
    cd receiver
    ```
2.  **Install MQTT Library:**
    ```sh
    pip install paho-mqtt
    ```
3.  **Update Credentials**: Open `receiver/final_receiver_ultrasonic.py`. Replace the placeholder values with **your own HiveMQ credentials**, ensuring they match the ESP8266 code.

---

## Usage

1.  **Power On and First-Time Setup**:
    -   Power on your ESP8266 while it's connected to your computer.
    -   Open the **Serial Monitor** in the Arduino IDE (Baud Rate: 115200).
    -   The ESP8266 will not find any saved credentials and search for the wifi and will show every SSID available and will prompt you to enter the **Password** for your network.
    -   Once you enter the credentials, the device will save them and connect automatically.

2.  **Normal Operation**:
    -   On subsequent power-ups, the ESP8266 will automatically connect to the saved Wi-Fi network and the MQTT broker. The on-board LEDs will light up to indicate a successful connection.

3.  **Run the Python Receiver**:
    -   Navigate to the `receiver` folder in your terminal and run the Python script:
        ```sh
        cd path/to/your/project/receiver
        python final_receiver_ultrasonic.py
        ```
    -   When prompted, type `start` and press Enter.

4.  **Monitor the Data**:
    -   The ESP8266 will begin publishing sensor data, which will appear in your Python console in real-time.
    -   
[▶️ Watch a Demonstration Video](https://drive.google.com/file/d/17krifS6K_uvakduxZT-LT-FsaElfaJ5a/view?usp=sharing)

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
