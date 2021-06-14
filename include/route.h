#include <Arduino.h>

// Route table struct
struct __attribute__((__packed__)) route {
  uint8_t node; // Who we can talk to
  uint8_t neighbour; // RF95 from
  uint8_t hops_to_sender;
  int16_t neighbour_rssi;
};

void UpdateRoutingTable(uint8_t);
