# Features
- Real-Time Data Monitoring: Access live data from temperature, smoke, and motion sensors via a web interface.
- Remote Control: Control connected LEDs from any device on the same network using a web browser.
- Wi-Fi Connectivity: Utilizes a Wi-Fi module to connect the Arduino board to a local network, enabling wireless communication.
- Web-Based Interface: Simple and intuitive web-based client for monitoring and control, compatible with any modern web browser.
- Modular Design: Easily extend the project by adding more sensors or actuators to the Arduino board.
# Components
- Arduino Board: Central microcontroller managing sensors and actuators.
- Wi-Fi Module: Connects the Arduino to the local network and acts as a server.
# Sensors:
- Temperature Sensor: Measures ambient temperature.
- Smoke Sensor: Detects smoke levels for fire safety.
- Motion Sensor: Senses movement in the vicinity.
- LEDs: Two LEDs controlled remotely to demonstrate actuation capabilities.
# How It Works
- Hardware Setup:

Connect the temperature, smoke, and motion sensors to the Arduino board.
Connect two LEDs to the Arduino for remote control.
Integrate the Wi-Fi module with the Arduino to enable network connectivity.
Software Configuration:

Upload the provided Arduino code to configure the board as a server.
Set up the web server on the Arduino to handle requests from the client (web browser).
Network Communication:

The Arduino and Wi-Fi module act as a server, receiving commands and sending sensor data.
A web browser on any device connected to the same network can communicate with the server to monitor sensor data and control the LEDs.
Getting Started
Clone the Repository:

# bash
Copy code
git clone https://github.com/iamklevy/Remote-IoT-data-monitoring-and-control-using-Wi-Fi.git
cd Remote-IoT-data-monitoring-and-control-using-Wi-Fi
Set Up Hardware:
Follow the included schematic and wiring instructions to connect the sensors, LEDs, and Wi-Fi module to the Arduino board.

Upload Code to Arduino:
Use the Arduino IDE to upload the provided code to your Arduino board.

Connect to Wi-Fi:
Configure the Wi-Fi module with your network credentials to establish a connection.

Access the Web Interface:
Open a web browser on any device connected to the same network and navigate to the IP address assigned to the Arduino server to start monitoring and controlling the devices.

# Contributions
Contributions are welcome! If you have improvements, bug fixes, or new features to add, please fork the repository and submit a pull request. For major changes, please open an issue first to discuss what you would like to change.

