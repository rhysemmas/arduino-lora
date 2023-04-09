#include "packet.h"

void WritePacket(unsigned char *p, Headers *h, Data *d) {
  // Serialize single byte variables into headers struct
  memcpy(&h->recipient, p, sizeof(h->recipient));
  p += sizeof(h->recipient);
  memcpy(&h->sender, p, sizeof(h->sender));
  p += sizeof(h->sender);
  memcpy(&h->hops, p, sizeof(h->hops));
  p += sizeof(h->hops);

  // Serialize our multibyte int into headers struct
  uint16_t host = ntohs(h->last_rssi);
  memcpy(&host, p, sizeof(host));
  p += sizeof(host);

  // Serialize our message into data struct
  memcpy(&d->message, p, sizeof(d->message));
  p += sizeof(d->message);
}

void ReadPacket(unsigned char *p, Headers *h, Data *d) {
  // Serialize single byte variables into outBuf
  memcpy(p, &h->recipient, sizeof(h->recipient));
  p += sizeof(h->recipient);
  memcpy(p, &h->sender, sizeof(h->sender));
  p += sizeof(h->sender);
  memcpy(p, &h->hops, sizeof(h->hops));
  p += sizeof(h->hops);

  // Serialize our multibyte int into outBuf
  uint16_t net = htons(h->last_rssi);
  memcpy(p, &net, sizeof(net));
  p += sizeof(net);

  // Serialize message into outBuf
  memcpy(p, &d->message, sizeof(d->message));
  p += sizeof(d->message);
}
