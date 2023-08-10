# RFID-Reader-using-RC522-with-STM32F103C8

**1. Introduction**

This project aims to create an RFID (Radio Frequency Identification) reader using the RC522 RFID module and the STM32F103C8 microcontroller. The RC522 module is widely used for RFID applications and supports various RFID tags. The STM32F103C8 is a popular microcontroller based on the ARM Cortex-M3 architecture.

**2. Hardware Components**

- STM32F103C8 microcontroller board
- RC522 RFID module
- RFID tags (compatible with RC522)
- Breadboard and jumper wires

**3. Software Tools**

- STM32CubeIDE: Integrated Development Environment for STM32 microcontrollers
- Standard Peripheral Library (SPL) or STM32Cube HAL: For STM32F1 series peripherals and GPIO management
- ST-LINK/V2: To program the STM32 microcontroller

**4. RC522 Library**

A library is developed for the RC522 module to provide functions for initializing the module, detecting RFID tags, performing anti-collision, and reading tag serial numbers.

**5. Project Implementation**

- Initialize SPI Peripheral and GPIO Pins: Configure the SPI peripheral and GPIO pins to communicate with the RC522 module. Set up the chip select (CS) pin and SPI communication parameters.

- RC522 Module Initialization: Perform a soft reset on the RC522 module during initialization and configure its registers as needed for the application.

- Request Tag Detection: Use the RC522_REQA command to request the detection of an RFID tag in proximity.

- Anti-Collision and Tag Identification: Implement the RC522_ANTICOLL command to perform anti-collision and retrieve the tag's serial number from the detected tags.

- Read Tag Serial Number: Use the RC522_TRANSCEIVE command to read the tag's serial number from the FIFO buffer.

**6. Main Function**

The main function sets up the necessary configurations, initializes the RC522 module, and continuously checks for the presence of RFID tags. When a tag is detected, it performs anti-collision and reads the tag's serial number, which is then displayed on the output.

**7. Testing and Verification**

Test the RFID reader with different RFID tags and verify that the tag serial numbers are correctly read and displayed on the output.

**8. Conclusion**

The RFID Reader using RC522 with STM32F103C8 project successfully demonstrates the interfacing of the RC522 RFID module with the STM32F103C8 microcontroller. The system can reliably read RFID tags and display their serial numbers, making it suitable for various applications, including access control, inventory management, and security systems. The project can be further extended to include additional features, such as data logging or interfacing with a database for more sophisticated applications.
