# SPI Communication with LSM6DSL IMU
## Overview
This project demonstrates the use of the SPI communication protocol to interface an LSM6DSL Inertial Measurement Unit (IMU) with an ATxmega128A1U microcontroller. The system collects 3D (XYZ) acceleration data from the IMU and transmits it to a USB serial port for real-time visualization. The microcontroller is configured to use SPI for data transmission and reception, with the LSM6DSL IMU sensor providing the acceleration data.

## Key Features:
- SPI communication with LSM6DSL IMU
- Real-time 3D acceleration data acquisition
- Visualization of sensor data through a USB serial interface
- Interrupt-driven accelerometer data collection
- Debugging and verification using logic analyzers

## Project Structure
The project's code is organized into several key files:

- lsm6dsl.c - Contains the main functions for interacting with the LSM6DSL sensor, including data read/write operations and initialization.
- lsm6dsl.h - Header file with macro definitions, custom data types, and function prototypes.
- spi.c - Provides SPI functionality for the ATxmega128A1U microcontroller, including initialization and data transmission.
- spi.h - Header file for SPI functions and macros.

## Hardware Requirements
- ATxmega128A1U microcontroller or any other device with an SPI interface
- LSM6DSL IMU sensor with SPI communication support
- Logic analyzer (optional for debugging)
- USB-to-serial interface for visualization (e.g., SerialPlot)

## Pin Connections
- MOSI (Master Out Slave In) - Connected to the LSM6DSL MOSI pin
- MISO (Master In Slave Out) - Connected to the LSM6DSL MISO pin
- SCK (Serial Clock) - Connected to the LSM6DSL SCK pin
- SS (Slave Select) - Connected to the LSM6DSL CS pin
- VCC and GND - Power and ground connections to the sensor

## Software Setup
1. SPI Initialization
The SPI communication is initialized using the spi_init() function defined in spi.c. It configures the relevant SPI pins (MOSI, MISO, SCK, SS) and sets the ATxmega128A1U's SPI settings (clock polarity, phase, LSB-first transmission, and baud rate).

2. LSM6DSL Initialization
The LSM_init() function initializes the LSM6DSL IMU by performing a software reset, configuring control registers, and setting up an interrupt for accelerometer data-ready events. The LSM_write() and LSM_read() functions allow interaction with the sensor’s registers to configure the IMU and retrieve data.

3. Data Acquisition
The system uses interrupts to trigger accelerometer measurements. Once an interrupt is triggered, the accelerometer's data is read in two-byte chunks and transmitted over USB using the transmitToUSB() function. Data is sent in a simple binary format to be visualized using SerialPlot or any other serial terminal.

4. Interrupt Service Routine (ISR)
The ISR is triggered upon receiving data from the accelerometer and sets the accel_flag to notify the main program loop that new data is available for transmission.

5. Data Transmission
The transmitToUSB() function sends the raw accelerometer data to the USB serial port. The data is sent as two bytes for each of the X, Y, and Z acceleration axes.

## Code Walkthrough
`lsm6dsl.c`
- LSM_write() - Sends a single byte of data to a register of the LSM6DSL IMU.
- LSM_read() - Reads a single byte of data from a register of the LSM6DSL IMU.
- LSM_init() - Initializes the LSM6DSL IMU with necessary configurations and settings.
- transmitToUSB() - Transmits accelerometer data to a USB serial port for real-time visualization.
- ISR(PORTC_INT0_vect) - Interrupt Service Routine triggered on accelerometer measurement completion.

`spi.c`
- spi_init() - Initializes the SPI module on the ATxmega128A1U microcontroller.
- spi_write() - Sends a byte of data via SPI.
- spi_read() - Receives a byte of data via SPI.

`spi.h`
Header file that defines SPI pin macros and function prototypes for spi_init(), spi_write(), and spi_read().

`lsm6dsl.h`
Header file that defines macros, custom data types for accelerometer and gyroscope data, and function prototypes for interacting with the LSM6DSL IMU.

## Real-time Data Visualization
The sensor data can be visualized in real time using SerialPlot (or any other serial terminal that supports binary data visualization). After connecting the microcontroller to your computer via a USB-to-serial interface, data can be displayed as graphs of the accelerometer's X, Y, and Z axis readings.
