#include <Arduino.h>
#include <Thread.h>

#include "handle.h"
#include "route.h"

// TODOS:
// moving everything rf95 related to the handler and using pointers everywhere
// has saved loads of memory - now need to debug the following problems:
// * received messages are not printed properly (need to check how we copy packets?)
// * strange loop after sending more than 1 message (things not being removed from queue?)

// Threads for handling message queues
Thread inThread = Thread();
Thread outThread = Thread();
Thread inputThread = Thread();

// Instantiate handle class
handle* h;

void receiveWrapper() {
  h->receive();
}

void sendWrapper() {
  h->send();
}

void scheduleMessageSend(String data) {
  char message[data.length()];
  data.toCharArray(message, sizeof(message));

  Packet pkt;
  pkt = NewPacket(message, 1, 2, 0);
  Serial.println(F("scheduling message!"));
  h->schedule(pkt);
}

void processInput() {
  String data;
  data = Serial.readString();

  if (data.length() > 0) {
    Serial.println(F("got input!"));
    scheduleMessageSend(data);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}; // Wait for serial port to be available

  h = new handle;

  inThread.onRun(receiveWrapper);
  inThread.setInterval(100);

  outThread.onRun(sendWrapper);
  outThread.setInterval(100);

  inputThread.onRun(processInput);
  inputThread.setInterval(100);
}

void loop() {
  if (inThread.shouldRun()) {
    inThread.run();
  }
  if (outThread.shouldRun()) {
    outThread.run();
  }
  if (inputThread.shouldRun()) {
    inputThread.run();
  }
}
