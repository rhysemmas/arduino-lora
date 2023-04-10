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
    Serial.println(F("Init failed"));
  }

  rf95.setFrequency(frequency);
  // rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95.setTxPower(13, false);

  // rf95.setCADTimeout(10000);
  rf95.setPromiscuous(true);

  Serial.print(F("Max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void sendMessage(uint8_t *message, size_t messageLen) {
  uint8_t data[messageLen];
  memcpy(data, message, messageLen);

  Serial.print("sending message: ");
  for (uint8_t i = 0; i < messageLen; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();

  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN] = {0};
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000)) {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len)) {
      Serial.print("got reply: ");
      Serial.println((char *)buf);
    } else {
      Serial.println("recv failed");
    }
  } else {
    Serial.println("No reply, is rf95_server running?");
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
      RH_RF95::printBuffer("request: ", buf, len);

      Serial.print("got request: ");
      Serial.println((char *)buf);

      // Send a reply
      uint8_t data[] = "hi tired, im dad";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(led, LOW);
    } else {
      Serial.println("recv failed");
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
    Serial.print("read bytes: ");
    Serial.println((char *)buffer);

    Serial.print("num bytes read: ");
    Serial.println(bufSize);

    sendMessage(buffer, bufSize);
  }
}
