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

#### Emacs Setup

* Follow [PlatformIO Instructions](https://docs.platformio.org/en/latest/integration/ide/emacs.html#integration)
* `pio project init --ide emacs`
* Install ccls
* TODO: fix - have to manually enter `platformio-mode` from editor to get build command

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

* Still getting garbage messages even after "fixing" memcpys that were the wrong way round - I don't know what I'm doing with pointers lol!!!

```
Message received: �~Ǐ��_>v�Q�/��>�o␡��u�␡ߧS����;��{��m���sQ-���طǛ���␖���v�}��l��␇m�͒]�3␛��Nޯ�}�␎ȫ{���V�����T�w�␡�f�^��|'u^:��jx��␏��{�����p��}w����ݝ��yF\]q�����w|�,��R?w��'�v���<�=�z�e��]��������u������Y����������s�~��
��^�5�7����2��mR�/�}��q�n�#Ҽ���w��������;␏���_��N�T�␘��FKG׉���o/��������\ٰ�_߫w��|��6�␡����._R��ջw��!�e{����Ϲ|�o���k����␋/␟&�������o9n������HS}/ҙ�Թ���␟�^�␝���x7����2���␡���␖��~}�׾␗�_'���␝����T�}␛_�����}␟��O���z�}���u�[�./��֥7��������_��;��m������O9���vw��������`�^��␡���c������␡:Ɵ���␇�������g���������׆z����������\�ʝ����[��{�␡N��s���_'���C��␁�ݼ�v��޾���{}��fO�␙���}�␞�␡�nW�␛�O5����[��x�����M�␜��������M��~1�����;<\��~k�p<�]R���NS��␎��a��:w�5�����y�[/�۴c␡�'t���␜b�9<����e�iOw␝���>����u�����.߁Ͽ�_}�V���������׿�[�n�t␖}ڤ�g�}␡����k����֟�r��~{�ξ�����hv_�i��␖����,������o��kÛ{␔�"��m���׮��,W�-�'���+��y���7��>;^�␗�w��~��w�45�-պ�>���~�Z��'�w׏���~�␏��}␜␓|␡�?�r�␖=���O����=l�����O����~X��=�ɫ˜��␡/���;sؿ�n�=�k␞o�p�œW��)�␡Eͫu���l������␞��␡��␞n���o�N��a���i�Ds�q��|�=��S�?���v��:Q�_�yN�&�}��|��?#���u���s��ƕ�␌6␂�␁�+-␃␃␄G$␀#�␀␂�␇�␇�␃t␂
```
