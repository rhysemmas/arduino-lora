#include <Arduino.h>
#include <RH_RF95.h>

// Packet flag consts - 4 bits to work with
const uint8_t ACK = 0b0000001;
const uint8_t HOP = 0b0000010;

struct __attribute__((__packed__)) Headers {
  uint8_t recipient; // Final recipient
  uint8_t sender; // Original sender
  uint8_t hops; // Hop counter
  uint16_t last_rssi;
};

struct __attribute__((__packed__)) Data {
  char message[(RH_RF95_MAX_MESSAGE_LEN-sizeof(struct Headers)-1)];
};

// Need to do dynamically sized data, so we dont have to send massive packets
struct __attribute__((__packed__)) Packet {
  Headers headers;
  Data data;
};

void ReadPacket(unsigned char* p, Packet pkt);

void WritePacket(unsigned char* p, Packet pkt);
