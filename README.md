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

## TODOs

* Right now we receive garbage messages - probably due to some poor copying to or from the radio:
```
Message available
Message received: v�[�e�����~����n���������{ܻ�v�߻���������<��
                                                            ��R��|����v<���ľ�SV�u��]�;�3wt�ݻy�Z{�����וW�ߤ�O~�J����vw��ڟ�����g������n�ƪ�ۦu�ھ�/�����=v��//�ݿ��g��s�7	YW�3�G�������ŷ7�N�����CfC
Sending ACK
Waiting for channel to clear
Sending message: ACK
```

* Packet structure/building/marshalling
* MAC - CSMA/CA
* Routing
* Better readme and a Makefile
