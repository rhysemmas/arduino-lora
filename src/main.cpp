#include <Arduino.h>
#include <RH_RF95.h>
#include <SPI.h>

#include "packet.h"
#include "route.h"

// Singleton instance of the radio driver
RH_RF95 rf95;

// ITSM Frequency
float frequency = 868.1;

// LED to blink on tx/rx
uint8_t led = LED_BUILTIN;

// Packet flag consts - 4 bits to work with
const uint8_t ACK = 0b0000001;
const uint8_t HOP = 0b0000010;
// const uint8_t RTR = 0b0000100;
// const uint8_t FRG = 0b0001000;

// RF95 headers
uint8_t FROM = 1; // Neighbour from
uint8_t TO = 2;   // Neighbour to

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for serial port to be available
  if (!rf95.init()) {
    Serial.println(F("Init failed"));
  }

  rf95.setFrequency(frequency);
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95.setTxPower(13, false);

  // Not sure how this is meant to work, when set the radio never detects the
  // channel as clear
  // rf95.setCADTimeout(10000);
  rf95.setHeaderTo(TO);
  rf95.setHeaderFrom(FROM);
  rf95.setThisAddress(FROM);
  rf95.setPromiscuous(true);

  Serial.print(F("Max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void sendMessage(char *message) {
  Headers *h = new Headers();
  h->recipient = TO;
  h->sender = FROM;
  h->hops = 0;

  Data *d = new Data;
  strcpy(d->message, message);

  unsigned char outBuf[RH_RF95_MAX_MESSAGE_LEN];
  unsigned char *p = outBuf;
  WritePacket(p, h, d);

  Serial.println(F("Waiting for channel to clear"));
  if (rf95.waitCAD()) {
    digitalWrite(led, HIGH);
    Serial.print(F("Sending message: "));
    Serial.println(message);

    rf95.send((const uint8_t *)outBuf, sizeof(outBuf));
    rf95.waitPacketSent();
    digitalWrite(led, LOW);
  }

  delete (h);
  delete (d);
}

void sendACK(char *ACKMessage, int messageSize) {
  if (rf95.waitCAD()) {
    Serial.println(F("Sending ACK"));

    rf95.setHeaderFlags(ACK);
    sendMessage(ACKMessage);

    // Clear the ACK header for the next message
    rf95.setHeaderFlags(0, ACK);
  }
}

void checkForMessages() {
  if (rf95.available()) {
    Headers *h = new Headers();
    Data *d = new Data;

    unsigned char buf[RH_RF95_MAX_MESSAGE_LEN];
    unsigned char len = sizeof(buf);
    unsigned char *p = buf;

    Serial.println(F("Message available"));

    if (rf95.recv(buf, &len)) {
      digitalWrite(led, HIGH);
      ReadPacket(p, h, d);
      Serial.print(F("Message received: "));
      Serial.println(d->message);
      digitalWrite(led, LOW);

      // Convert headers back to struct and update routing table - or do this as
      // part of routing table update
      // updateRoutingTable(headers);

      // Send an ACK if we have not received an ACK
      if (rf95.headerFlags() != 1) {
        char ACKMessage[] = "ACK";
        sendACK(ACKMessage, sizeof(ACKMessage));
      }
    } else {
      Serial.println(F("recv function failed"));
    }

    delete (h);
    delete (d);
  }
}

void waitForACK() {
  if (rf95.waitAvailableTimeout(30000)) {
    checkForMessages();
  } else {
    Serial.println(F("No reply"));
  }
}

void loop() {
  // Enter listen mode
  // TODO: we need to listen for messages (in a thread?) and when we get one,
  // we need to send an ACK along with any other information.
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  String data;
  data = Serial.readString();

  if (data.length() > 0) {
    char message[data.length()];
    data.toCharArray(message, sizeof(message));

    // TODO: when we have a message to send, we need to send it and then keep
    // track of it being sent and wait for a reply (ACK) so we know it was
    // received
    sendMessage(message);
    // Wait for an ACK from our message
    waitForACK();
  }
}
