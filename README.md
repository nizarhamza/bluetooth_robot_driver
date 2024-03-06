# Bluetooth Driving Robot with STM32F407VG

## Overview
This project showcases a Bluetooth-controlled robot powered by the STM32F407VG microcontroller. The robot's movements are controlled wirelessly via Bluetooth communication, offering an interactive and versatile platform for exploration and experimentation.

## Features
- **STM32F407VG Chip:** The project utilizes the powerful STM32F407VG microcontroller for robust and efficient control.
- **Bluetooth Communication:** Enables wireless control of the robot through Bluetooth, providing flexibility and ease of use.
- **Access Register Programming:** The project emphasizes programming with access registers, offering a low-level understanding of the STM32F407VG chip for enthusiasts and learners.
- **CoIDE Development Environment:** Developed using CoIDE, a feature-rich integrated development environment, ensuring a smooth and efficient development process.

## Hardware Requirements
- STM32F407VG Development Board
- Bluetooth Module (e.g., HC-05 or HC-06)
- Motor Driver
- Motors and Wheels
- Power Supply

## Software Requirements
- CoIDE (or any IDE supporting STM32 development)
- Bluetooth Terminal App (for remote control)

## Getting Started
1. Clone the repository to your local machine.
   ```bash
   git clone https://github.com/nizarhamza/bluetooth_robot_driver
2. Open the project in CoIDE.

3. Connect the STM32F407VG development board to your computer.

4. Ensure proper wiring of motors and peripherals.

5. Build and flash the code onto the STM32F407VG.

6. Power up the robot and establish a Bluetooth connection with the controlling device.

Use the Bluetooth terminal app to send commands and drive the robot!

## Code Structure
The code is organized into logical sections, emphasizing the use of access registers for programming the STM32F407VG. Key sections include:

Initialization and Configuration
Bluetooth Communication Setup ( USART )

## Contribution
Contributions are welcome! If you find any issues or have enhancements to suggest, feel free to create a pull request.


## Acknowledgements
Special thanks to the STM32 and CoIDE communities for their valuable resources and support.

Happy robot driving!

