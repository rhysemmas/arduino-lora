#include <Arduino.h>
#include <RH_RF95.h>

// Can we dynamically size the array based on the message we're sending
// - set a header which the receiver reads to decide how long the buffer will be
#define buffer_size RH_RF95_MAX_MESSAGE_LEN // Max message size: 251

struct __attribute__((__packed__)) headers {
  uint8_t recipient; // Final recipient
  uint8_t sender; // Original sender
  uint8_t hops; // Hop counter
  uint16_t last_rssi;
};

struct __attribute__((__packed__)) data {
  char message[(buffer_size-sizeof(struct headers)-1)];
};

void readPacket(unsigned char* p, headers h, data d);
