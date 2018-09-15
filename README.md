# iot-m5stack
Getting started with M5 Stack ESP32

## Installing the ESP32 Board in Arduino IDE
[Tutorial bt Random Nerd](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

1. Install the [ESP32 CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). Do not use the Universal Windows 10 installer, but the [Windows 7/8/8.1 (v6.7.6)](https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip)
1. If success, you should see a new COM port (*Silicon Labs CP210x USB to UART Bridge*) when plugging M5 Stack to the PC

1. Installing the ESP32 Board
  - In Arduino IDE go to *File -> Preferences*
  - Enter https://dl.espressif.com/dl/package_esp32_index.json into the *“Additional Board Manager URLs”* (do not delete the existing URL in the input box- i.e. ESP8266-, add this one after a comma)
  - Open boards manager going to *Tools > Board > Boards Manager…*
  - Search for ESP32 and press install button for the “ESP32 by Espressif Systems“
1. Select board **M5Stack-Core-ESP32**

## M5Stack with DHT type sensors
[Reference tutorial in Instructables](https://www.instructables.com/id/ESP32-M5Stack-With-DHT22/)

1. Add to Arduino IDE the library for the DHT sensors: [**SimpleDHT** by *Winlin*](https://github.com/winlinvip/SimpleDHT)

1. Install the Arduino IDE Library for M5 Stack: [**M5Stack** by *M5Stack*](Arduino IDE Library)

1. Install library [**M5Stack-SD-Updater**](https://github.com/tobozo/M5Stack-SD-Updater/)

1. Restart Arduino IDE so that new libraries are loaded and known to the IDE
