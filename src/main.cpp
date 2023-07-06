#include <Arduino.h>
#include <RH_RF95.h>
#include <SPI.h>

#include "packet.cpp"

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
  rf95.setTxPower(23);
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95.setLowDatarate();
  rf95.setPromiscuous(true);
  // rf95.setCADTimeout(10000);

  // set this node's ID
  Serial.print(F("enter node ID: "));
  Serial.println();
  uint8_t buffer[1] = {0};
  size_t bufSize = 0;

  if (Serial.available() > 0) {
    bufSize = Serial.readBytesUntil('\n', buffer, sizeof(buffer));
    if (bufSize > 0) {
      rf95.setHeaderFrom(buffer[0]);
    }
  }

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
  uint8_t len = sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN;
  uint8_t *buf = (uint8_t *)malloc(len);
  memset(buf, 0, len);

  Serial.println(F("waiting for reply..."));

  if (rf95.waitAvailableTimeout(10000)) {
    receive(buf, &len);
  } else {
    Serial.println(F("no reply, is anyone there?"));
  }

  free(buf);
}

void checkForMessages() {
  uint8_t len = sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN;
  uint8_t *buf = (uint8_t *)malloc(len);
  memset(buf, 0, len);

  // If we have a message, send a reply
  if (receive(buf, &len)) {
    uint8_t reply[len + 7];
    strcpy((char *)reply, (char *)buf);
    strcat((char *)reply, " - no u");

    send(reply, sizeof(reply));
  }

  free(buf);
}

void loop() {
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  uint8_t buffer[RH_RF95_MAX_MESSAGE_LEN] = {0};
  size_t bufSize = 0;
  bufSize = Serial.readBytesUntil('\n', buffer, sizeof(buffer));

  if (bufSize > 0) {
    // get TO header
    Serial.print(F("enter recipient ID: "));
    Serial.println();

    uint8_t buf[1] = {0};
    size_t bufS = 0;
    if (Serial.available() > 0) {
      bufS = Serial.readBytesUntil('\n', buf, sizeof(buf));
      if (bufS > 0) {
        rf95.setHeaderTo(buf[0]);
        Serial.print(F("sending message to: "));
        Serial.println(buf[0]);
      }
    }
    send(buffer, bufSize);
    waitForReply();
  }
}
