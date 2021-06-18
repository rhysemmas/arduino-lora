#include "packet.h"

Packet NewPacket(const char* message, int to, int from, int hops) {
  struct Packet pkt;
  pkt.headers.recipient = to;
  pkt.headers.sender = from;
  pkt.headers.hops = hops;

  strcpy(pkt.data.message, message);
  return pkt;
}

void ReadPacket(unsigned char* p, Packet pkt) {
  // Serialize single byte variables into headers struct
  memcpy(&pkt.headers.recipient, p, sizeof(pkt.headers.recipient));
  p += sizeof(pkt.headers.recipient);
  memcpy(&pkt.headers.sender, p, sizeof(pkt.headers.sender));
  p += sizeof(pkt.headers.sender);
  memcpy(&pkt.headers.hops, p, sizeof(pkt.headers.hops));
  p += sizeof(pkt.headers.hops);

  // Serialize our multibyte int into headers struct
  uint16_t host = ntohs(pkt.headers.last_rssi);
  memcpy(&host, p, sizeof(host));
  p += sizeof(host);

  // Serialize our message into data struct
  memcpy(&pkt.data.message, p, sizeof(pkt.data.message));
  p += sizeof(pkt.data.message);
}

void WritePacket(unsigned char* p, Packet pkt) {
  // Serialize single byte variables into outBuf
  memcpy(p, &pkt.headers.recipient, sizeof(pkt.headers.recipient));
  p += sizeof(pkt.headers.recipient);
  memcpy(p, &pkt.headers.sender, sizeof(pkt.headers.sender));
  p += sizeof(pkt.headers.sender);
  memcpy(p, &pkt.headers.hops, sizeof(pkt.headers.hops));
  p += sizeof(pkt.headers.hops);

  // Serialize our multibyte int into outBuf
  uint16_t net = htons(pkt.headers.last_rssi);
  memcpy(p, &net, sizeof(net));
  p += sizeof(net);

  // Serialize message into outBuf
  memcpy(p, &pkt.data.message, sizeof(pkt.data.message));
  p += sizeof(pkt.data.message);
}
