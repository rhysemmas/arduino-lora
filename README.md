# Arduino LoRa

LoRa radio mesh using Arduino microcontrollers.

## Hardware

TODO - links

* Arduino Uno
* Dragino LoRa Shield
* Antenna

## Install

### Prerequisites

* [arduino-cli](https://arduino.github.io/arduino-cli/0.32/installation/)
  * arduino:avr - `arduino-cli core install arduino:avr`

### Run

Note: each node needs its ID flashing to it, this is changed in `main.cpp` manually right now (sorry)

Build Sketch:
`arduino-cli compile -b arduino:avr:uno`

Find Board:
`arduino-cli board list`

Upload Sketch:
`arduino-cli upload -b aruidno:avr:uno -p /dev/ttyACM0`

Open Monitor:
`arduino-cli monitor -p /dev/ttyACM0`
