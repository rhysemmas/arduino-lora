#include "packet.h"

void readPacket(unsigned char* p, headers h, data d) {
  // Serialize single byte variables into headers struct
  memcpy(&h.recipient, p, sizeof(h.recipient));
  p += sizeof(h.recipient);
  memcpy(&h.sender, p, sizeof(h.sender));
  p += sizeof(h.sender);
  memcpy(&h.hops, p, sizeof(h.hops));
  p += sizeof(h.hops);

  // Serialize our multibyte int into headers struct
  uint16_t host = ntohs(h.last_rssi);
  memcpy(&host, p, sizeof(host));
  p += sizeof(host);

  // Serialize our message into data struct
  memcpy(&d.message, p, sizeof(d.message));
  p += sizeof(d.message);
}
