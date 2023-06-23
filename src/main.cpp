#include <Arduino.h>
#include <RH_RF95.h>
#include <SPI.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

// ITSM Frequency
float frequency = 868.1;

// LED to blink on tx/rx
uint8_t led = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for serial port to be available

  if (!rf95.init()) {
    Serial.println(F("init failed"));
  }

  rf95.setFrequency(frequency);
  rf95.setTxPower(23, false);
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

  // RH_RF95::ModemConfig modem_config = {
  //     0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
  //     0xc4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
  //     0x0c  // Reg 0x26: LowDataRate=On, Agc=On
  // };
  // rf95.setModemRegisters(&modem_config);

  // rf95.setCADTimeout(10000);
  rf95.setPromiscuous(true);

  Serial.print(F("max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void send(uint8_t *message, size_t messageLen) {
  uint8_t data[messageLen];
  memcpy(data, message, messageLen);

  digitalWrite(led, HIGH);
  Serial.print(F("sending message: "));
  for (uint8_t i = 0; i < messageLen; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();

  Serial.print(F("size of message: "));
  Serial.println(sizeof(data));

  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  digitalWrite(led, LOW);
}

boolean receive(uint8_t *buf, uint8_t *len) {
  if (rf95.available()) {
    if (rf95.recv(buf, len)) {
      digitalWrite(led, HIGH);

      rf95.printBuffer("receive buffer: ", buf, *len);

      Serial.print(F("got message: "));
      Serial.println((char *)buf);

      Serial.print(F("size of message: "));
      Serial.println(*len);

      Serial.print(F("rssi: "));
      Serial.println(rf95.lastRssi());

      Serial.print(F("snr: "));
      Serial.println(rf95.lastSNR());

      digitalWrite(led, LOW);

      return true;
    } else {
      Serial.println(F("recv failed"));
    }
  }
  return false;
}

void waitForReply() {
  Serial.print(F("allocating size for buffer: "));
  Serial.println(sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN);

  uint8_t *buf = (uint8_t *)malloc(sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN);
  uint8_t *len = (uint8_t *)malloc(sizeof(uint8_t));

  if (rf95.waitAvailableTimeout(10000)) {
    receive(buf, len);
  } else {
    Serial.println(F("no reply, is anyone there?"));
  }
}

void checkForMessages() {
  uint8_t *buf = (uint8_t *)malloc(sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN);
  uint8_t *len = (uint8_t *)malloc(sizeof(uint8_t));

  if (receive(buf, len)) {
    uint8_t reply[*len + 7];
    strcpy((char *)reply, (char *)buf);
    strcat((char *)reply, " - no u");

    send(reply, sizeof(reply));
  }
}

void loop() {
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  uint8_t buffer[RH_RF95_MAX_MESSAGE_LEN] = {0};
  size_t bufSize = 0;
  bufSize = Serial.readBytes(buffer, sizeof(buffer));

  if (bufSize > 0) {
    send(buffer, bufSize);
    waitForReply();
  }
}
