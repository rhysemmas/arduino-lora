#include <RH_RF95.h>
#include <SPI.h>

#include "handle.h"

// ITSM Frequency
float frequency = 868.1;

// RF95 headers
uint8_t FROM = 2; // Neighbour from
uint8_t TO = 1; // Neighbour to

// Constructor to initialize a handle
handle::handle() {
  rf95 = new RH_RF95;
  rf95->init();
  Serial.println(F("max message length: 64"));
  // Serial.println(rf95->maxMessageLength());

  // Not sure how this is meant to work, when set the radio never detects the channel as clear
  //rf95->setCADTimeout(10000);
  rf95->setFrequency(frequency);
  rf95->setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95->setTxPower(13, false);
  rf95->setHeaderTo(TO); // we can set this when sending?
  rf95->setHeaderFrom(FROM); // we can set this when sending?
  rf95->setThisAddress(FROM);
  rf95->setPromiscuous(true);

  in = new linked_list;
  out = new linked_list;
}

// Destructor to free memory allocated to the lists by the handle
handle::~handle() {
  delete in;
  delete out;
}

void handle::receive() {
  if (!rf95->available()) {
    return;
  }

  Packet pkt;
  unsigned char inBuf[64];
  unsigned char len = sizeof(inBuf);
  unsigned char* p = inBuf;

  if (rf95->recv(inBuf, &len)) {
    ReadPacket(p, pkt);
    Serial.print(F("message received: "));
    Serial.println(pkt.data.message);
    in->add_node(pkt);
    return;
  }
  Serial.println(F("recv function failed"));
}

void handle::send() {
  Serial.print(F("current list length: "));
  Serial.println(out->size());
  if (out->is_empty()) {
    return;
  }

  Packet pkt;
  pkt = out->peek();

  unsigned char outBuf[64]; // sizeof(packet)?
  unsigned char* p = outBuf;
  WritePacket(p, pkt);

  if (rf95->waitCAD()) {
    Serial.println(F("sending..."));
    rf95->send((const uint8_t*)outBuf, sizeof(outBuf));
    rf95->waitPacketSent(1000);
    out->delete_node(pkt);
    Serial.println(F("sent!"));
  }
}

bool handle::read() {
  Packet pkt;
  pkt = in->peek();
  in->delete_node(pkt);

  // we need to send the packet to some route table building function
  // at some point we also need to decide if this packet requires an ACK from us
  // in the future we will also decide if the packet is destined for someone else
  return true;
}

// this function is currently pointless
bool handle::schedule(Packet pkt) {
  out->add_node(pkt);

  // Packet current_head;
  // current_head = out->peek();
  // Serial.print(F("sending message: "));
  // Serial.println(current_head.data.message);

  return true;
}
