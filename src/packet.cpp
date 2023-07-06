#include <Arduino.h>

struct header {
  uint8_t source;
  uint8_t destination;
  uint8_t nextHop;
  uint8_t lastHop;
  uint8_t numberOfHops;
  uint8_t lastRSSI;
};
