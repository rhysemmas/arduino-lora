#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "handle.h"
#include "route.h"

// Singleton instance of the radio driver
RH_RF95 rf95;

// Setup message handler
handle h(rf95);

// ITSM Frequency
float frequency = 868.1;

// RF95 headers
uint8_t FROM = 2; // Neighbour from
uint8_t TO = 1; // Neighbour to

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to be available
  if (!rf95.init()) {
    Serial.println(F("init failed"));
  }

  // Not sure how this is meant to work, when set the radio never detects the channel as clear
  //rf95.setCADTimeout(10000);
  rf95.setFrequency(frequency);
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95.setTxPower(13, false);
  rf95.setHeaderTo(TO);
  rf95.setHeaderFrom(FROM);
  rf95.setThisAddress(FROM);
  rf95.setPromiscuous(true);

  Serial.print(F("max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void checkForMessages() {
  if (rf95.available()) {
    Serial.println(F("message available"));
    h.receive();
  }
}

void scheduleMessageSend(String data) {
  char message[data.length()];
  data.toCharArray(message, sizeof(message));

  Packet pkt;
  pkt = NewPacket(message, TO, FROM, 0);
  h.schedule(pkt);
}

void loop() {
  // TODO: we should run this in a protothread and interrupt whenever a message
  // is available, so we can add it to our in queue
  // or can we just wait until the loop starts again and checkForMessages runs?
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  String data;
  data = Serial.readString();
  if (data.length() > 0) {
    // TODO: when we have a message to send, we should schedule it into our out
    // queue, we can then have something that actually sends the message when
    // the radio is not busy
    scheduleMessageSend(data);

    // TODO: once we have sent a message, we need to keep track of it
    // and wait for a reply (ACK) so we know it was received
  }
}
