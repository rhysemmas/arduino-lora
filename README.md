# Arduino LoRa

LoRa radio mesh using Arduino microcontrollers.

## Hardware

TODO - links

* Arduino Uno
* Dragino LoRa Shield
* Antenna

## Install

### Prerequisites

The project uses [PlatformIO](https://docs.platformio.org/en/latest/integration/ide/index.html) which comes with a GUI/mode for making building and uploading sketches in your favourite IDE easier.

* [PlatformIO Core](https://docs.platformio.org/en/latest/core/index.html#piocore)

### Running

Note: each node needs its ID flashing to it, this is changed in `main.cpp` manually right now (sorry)

PlatformIO includes tools to build and upload code to the Arduino Uno. You can also use it to open a serial monitor to the device from the CLI with `pio device monitor` 
