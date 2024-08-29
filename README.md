# [DES] Instrument-Cluster

more information - https://github.com/SEA-ME/DES_Instrument-Cluster
<br>
- [Introduction](#introduction)
- [Features and Key Components](#features-and-key-components)
- [Hardware Components](#hardware-components)
- [Software Design](#software-design)
	- [System Architecture](#system-architecture)
	- [Module Descriptions](#module-descriptions)
- [Installation and Usage](#installation-and-usage)


<br>

---

## Introduction

The PiRacer Instrument Cluster Qt Application is a comprehensive project aimed at developing a real-time speedometer and vehicle status dashboard for the PiRacer car. This application, running on a Raspberry Pi, will visualize speed data collected from an in-vehicle speed sensor through the Controller Area Network (CAN) protocol, a widely-used communication standard in the automotive industry.

Beyond speed monitoring, the project also features an additional module that leverages the I2C protocol to monitor and display the vehicle's battery status within the same Qt-based application. This dual-protocol integration provides a robust platform for real-time vehicle telemetry.

This project not only offers a hands-on experience in embedded systems and software architecture but also deepens understanding of communication protocols and GUI frameworks like Qt, which are pivotal in developing modern automotive applications. By successfully completing this project, participants will demonstrate their ability to design, implement, and communicate complex software solutions in a real-world context, preparing them for advanced roles in software engineering, particularly in the automotive and embedded systems domains.

</br>

## Features and Key Components
- Speed Sensor Data Collection: Gathers vehicle speed data using an Arduino CAN shield.
- Data Transmission and Communication: Transmits data to Raspberry Pi through the CAN bus and CAN HAT.
- Qt Application: Receives and visualizes the collected data in real-time on a dashboard.
- I2C Communication (Bonus Feature): Gathers vehicle battery data via I2C and displays it on the Qt app.
<br>

---

## Hardware Components
- **az-delivery UNO**: The microcontroller board used to interface with the speed sensor and handle CAN communication.
-  **CAN-BUS Shield V2.0** (seeed studio) or **MCP2515**: Required to enable CAN communication between the Raspberry Pi and Arduino.
- **Raspberry Pi 4** Model B 8GB RAM: The main computing platform where the Qt application runs and processes the received data.
- **CAN-BUS (FD) HAT for Raspberry Pi** (seeed studio): Enables CAN communication on the Raspberry Pi, allowing it to receive data from the Arduino.
- **Speed Sensor**: The sensor that captures vehicle speed data, which is then transmitted via CAN bus to the Raspberry Pi.
<br>

## Software Design
- **az-delivery UNO**
- **CAN-BUS Shield V2.0** (seeed studio) or **MCP2515**
- **Raspberry Pi 4** Model B 8GB RAM
- **CAN-BUS (FD) HAT for Raspberry Pi** (seeed studio**)
- **speed sensor**
<br>

### System Architecture
```
[Speed Sensor] -> [Arduino CAN Shield] -> [CAN Bus] -> [Raspberry Pi CAN HAT] -> [Qt Application]
[I2C Battery Module] -> [Raspberry Pi] -> [Qt Application]
```
<br>

### Module Descriptions
Arduino Module:
- **CAN Shield**: Collects speed sensor data from the vehicle and transmits it via the CAN protocol to the Raspberry Pi.
- **I2C Communication**: In the bonus part, the Arduino collects battery data via I2C.

Raspberry Pi:
- **CAN HAT**: Receives data from the CAN shield and relays it to the Raspberry Pi.
- **I2C Interface**: Directly collects battery data and forwards it to the Qt application.

Qt Application:
- **Dashboard UI**: Visualizes the received speed and battery data in real-time.
- **Data Communication**: Receives CAN and I2C data from the Raspberry Pi and processes it.
<br>
---

## Installation and Usage

### Requirements
- **Qt Framework**
- **Arduino IDE**
- **Raspberry Pi Setup**
	- CAN driver configuration
		```
		sudo vim /boot/firmware/config.txt
		```
		Add the following line to the end of the config.txt file
		```
		dtoverlay=seeed-can-fd-hat-v2
		```
		```
		sudo reboot
		apt-get install can-utils
		sudo ip link set can0 up type can bitrate 500000
		```
	- Enable I2C interface

### Usage
**This compilation is only possible on ARM CPUs.**

1. Modify the file to suit your own settings.
2. Run the a.sh script.
2. Execute the execute file in raspberry file.

<br>




![falcon gif](/image/falcon.gif)
![falcon image](/image/falcon0.jpeg)
![falcon image](/image/falcon1.jpeg)
![falcon image](/image/falcon2.jpeg)

</br>
