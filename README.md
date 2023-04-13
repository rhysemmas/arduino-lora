# Arduino LoRa

LoRa radio mesh using Arduino microcontrollers.

## Project TODOs

* Packet structure/building/marshalling
* MAC - CSMA/CA
* Routing
* Better readme and a Makefile

## Hardware

* [Arduino Uno Compatible Board](https://www.amazon.co.uk/ELEGOO-Arduino-Arduino-Compatible-Transfer-Operation/dp/B09JWFTZ2V/ref=sr_1_3?crid=GYB1ZQYGWVF5&keywords=elegoo+uno&qid=1681422688&sprefix=elegoo+uno%2Caps%2C118&sr=8-3)
* [Dragino LoRa Shield](https://uk.robotshop.com/products/dragino-lora-transceiver-shield)
* [Antenna](https://www.amazon.co.uk/Paradar-LoraWAN-connector-aviation-Software-Black/dp/B08WJ9LRMB/ref=sr_1_5?crid=3KH3CZ8KLQY80&keywords=lora+868+antenna&qid=1681423398&sprefix=lora+868+antenna%2Caps%2C95&sr=8-5) - note we could use more powerful outdoor antennas instead

## Install

### Prerequisites

The project uses [PlatformIO](https://docs.platformio.org/en/latest/integration/ide/index.html) which comes with a GUI/mode for making building and uploading sketches in your favourite IDE easier. There are some useful links for some editors below.

* [PlatformIO Core](https://docs.platformio.org/en/latest/core/index.html#piocore)

#### VSCode + WSL Setup

* Install the [PlatformIO IDE integration](https://docs.platformio.org/en/latest/integration/ide/vscode.html) for VSCode running in WSL mode
* Install the [udev rules for PlatformIO](https://docs.platformio.org/en/stable//core/installation/udev-rules.html) in your WSL instance
* Follow the steps to [passthrough USB devices from Windows to WSL](https://learn.microsoft.com/en-us/windows/wsl/connect-usb) - you will need to passthrough the Arduino board USB device every time you reconnect it!

#### Emacs Setup

* Follow [PlatformIO Instructions](https://docs.platformio.org/en/latest/integration/ide/emacs.html#integration)
* `pio project init --ide emacs`
* Install [ccls for lsp-mode](https://emacs-lsp.github.io/lsp-mode/page/lsp-ccls/)
* TODO: fix - have to manually enter `platformio-mode` from editor to get build command as by default we just drop into c++ mode

### Running

PlatformIO includes tools to build and upload code to the Arduino Uno. You can also use it to open a serial monitor to the device from the CLI with `pio device monitor` 

With the monitor attached, you can send messages to any other listening arduino-lora nodes!
