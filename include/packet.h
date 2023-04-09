#include <Arduino.h>
#include <RH_RF95.h>

struct __attribute__((__packed__)) Headers {
  uint8_t recipient; // Final recipient
  uint8_t sender;    // Original sender
  uint8_t hops;      // Hop counter
  uint16_t last_rssi;
};

// Need to do dynamically sized data, so we dont have to send massive packets
struct __attribute__((__packed__)) Data {
  char message[(RH_RF95_MAX_MESSAGE_LEN - sizeof(struct Headers) - 1)];
};

void ReadPacket(unsigned char *p, Headers *h, Data *d);

void WritePacket(unsigned char *p, Headers *h, Data *d);
