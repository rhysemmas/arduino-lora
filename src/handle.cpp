#include "handle.h"

// TODO
// send and read functions need to be run on a schedule so we can work through
// the queues of packets
// do we need to interrupt whenever we want to send or receive closer to the
// radio driver level?

// Constructor to initialize a handle
handle::handle(RH_RF95 driver) {
  RH_RF95(rf95) = driver;
  in = new queue(10);
  out = new queue(10);
}

// Destructor to free memory allocated to the queues by the handle
handle::~handle() {
  delete in;
  delete out;
}

bool handle::receive() {
  Packet pkt;
  unsigned char inBuf[RH_RF95_MAX_MESSAGE_LEN];
  unsigned char len = sizeof(inBuf);
  unsigned char* p = inBuf;

  if (rf95.recv(inBuf, &len)) {
    ReadPacket(p, pkt);
    Serial.print(F("message received: "));
    Serial.println(pkt.data.message);

    in->enqueue(pkt); // what do we do if the queue is full?
    return true;
  }
  Serial.println(F("recv function failed"));
  return false;
}

void handle::read() {
  Packet pkt;
  pkt = in->peek();
  in->dequeue();

  // we need to send the packet to some route table building function
  // at some point we also need to decide if this packet requires an ACK from us
  // in the future we will also decide if the packet is destined for someone else
}

bool handle::schedule(Packet pkt) {
  out->enqueue(pkt); // what do we do if the queue is full?
  return true;
}

void handle::send() {
  Packet pkt;
  pkt = out->peek();
  out->dequeue();

  unsigned char outBuf[RH_RF95_MAX_MESSAGE_LEN]; // sizeof(packet)?
  unsigned char* p = outBuf;
  WritePacket(p, pkt);

  Serial.println(F("waiting for channel to clear"));
  if (rf95.waitCAD()) {
    rf95.send((const uint8_t*)outBuf, sizeof(outBuf));
    rf95.waitPacketSent();
  }
}
