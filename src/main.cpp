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
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95.setTxPower(23, false);

  // rf95.setCADTimeout(10000);
  rf95.setPromiscuous(true);

  Serial.print(F("max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void sendMessage(uint8_t *message, size_t messageLen) {
  uint8_t data[messageLen];
  memcpy(data, message, messageLen);

  Serial.print(F("sending message: "));
  for (uint8_t i = 0; i < messageLen; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();

  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN] = {0};
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(10000)) {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len)) {
      Serial.print(F("got reply: "));
      Serial.println((char *)buf);

      Serial.print(F("rssi: "));
      Serial.println(rf95.lastSNR());

      Serial.print(F("snr: "));
      Serial.println(rf95.lastSNR());
    } else {
      Serial.println(F("recv failed"));
    }
  } else {
    Serial.println(F("no reply, is anyone there?"));
  }
}

void checkForMessages() {
  if (rf95.available()) {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN] = {0};
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
      digitalWrite(led, HIGH);

      // debug
      // RH_RF95::printBuffer("request: ", buf, len);

      Serial.print(F("got request: "));
      Serial.println((char *)buf);

      Serial.print(F("rssi: "));
      Serial.println(rf95.lastRssi());

      Serial.print(F("snr: "));
      Serial.println(rf95.lastSNR());

      // Send a reply
      uint8_t reply[len + 20];

      strcpy((char *)reply, (char *)buf);
      strcat((char *)reply, " - is what you said");

      rf95.send(reply, sizeof(reply));
      rf95.waitPacketSent();
      Serial.println(F("sent a reply"));
      digitalWrite(led, LOW);
    } else {
      Serial.println(F("recv failed"));
    }
  }
}

void loop() {
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  uint8_t buffer[RH_RF95_MAX_MESSAGE_LEN] = {0};
  size_t bufSize = 0;
  bufSize = Serial.readBytes(buffer, sizeof(buffer));

  if (bufSize > 0) {
    Serial.print(F("read bytes: "));
    Serial.println((char *)buffer);

    Serial.print(F("num bytes read: "));
    Serial.println(bufSize);

    sendMessage(buffer, bufSize);
  }
}
