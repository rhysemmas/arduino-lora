#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

// ITSM Frequency
float frequency = 868.1;

// LED to blink on tx/rx
uint8_t led = LED_BUILTIN;

// Packet flag consts - 4 bits to work with
const uint8_t ACK = 0b0000001;
const uint8_t HOP = 0b0000010;
//const uint8_t RTR = 0b0000100;
//const uint8_t FRG = 0b0001000;

// RF95 headers
uint8_t FROM = 2; // Neighbour from
uint8_t TO = 1; // Neighbour to

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

// Route table struct
struct __attribute__((__packed__)) route {
  uint8_t node; // Who we can talk to
  uint8_t neighbour; // RF95 from
  uint8_t hops_to_sender;
  int16_t neighbour_rssi;
};

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to be available
  if (!rf95.init()) {
    Serial.println(F("Init failed"));
  }

  rf95.setFrequency(frequency);
  rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
  rf95.setTxPower(13, false);

  // Not sure how this is meant to work, when set the radio never detects the channel as clear
  //rf95.setCADTimeout(10000);
  rf95.setHeaderTo(TO);
  rf95.setHeaderFrom(FROM);
  rf95.setThisAddress(FROM);
  rf95.setPromiscuous(true);

  Serial.print(F("Max message length: "));
  Serial.println(rf95.maxMessageLength());
}

void sendMessage(char* message) {
  struct headers h;
  h.recipient = TO;
  h.sender = FROM;
  h.hops = 0;

  struct data d;
  strcpy(d.message, message);
  //printf("message size: %lu\n", sizeof(d.message));

  // The buffer we will be writing bytes into
  //unsigned char outBuf[sizeof(h.recipient)+sizeof(h.sender)+sizeof(h.hops)+sizeof(h.last_rssi)+sizeof(message)];
  unsigned char outBuf[buffer_size];

  // A pointer we will advance whenever we write data
  unsigned char* p = outBuf;

  // Serialize single byte variables into outBuf
  memcpy(p, &h.recipient, sizeof(h.recipient));
  p += sizeof(h.recipient);
  memcpy(p, &h.sender, sizeof(h.sender));
  p += sizeof(h.sender);
  memcpy(p, &h.hops, sizeof(h.hops));
  p += sizeof(h.hops);

  // Serialize our multibyte int into outBuf
  uint16_t net = htons(h.last_rssi);
  memcpy(p, &net, sizeof(net));
  p += sizeof(net);

  // Serialize message into outBuf
  memcpy(p, &d.message, sizeof(d.message));
  p += sizeof(d.message);

  Serial.println(F("Waiting for channel to clear"));
  if (rf95.waitCAD()) {
    Serial.print(F("Sending message: "));
    Serial.println(message);

    rf95.send((const uint8_t*)outBuf, sizeof(outBuf));
    rf95.waitPacketSent();
  }
}

void sendACK(char* ACKMessage, int messageSize) {
  if (rf95.waitCAD()) {
    Serial.println(F("Sending ACK"));

    rf95.setHeaderFlags(ACK);
    sendMessage(ACKMessage);

    // Clear the ACK header for the next message
    rf95.setHeaderFlags(0, ACK);
  }
}

void checkForMessages() {
  if (rf95.available()) {
    struct headers h;
    struct data d;
    unsigned char buf[buffer_size];
    unsigned char len = sizeof(buf);
    unsigned char * p = buf;

    Serial.println(F("Message available"));

    if (rf95.recv(buf, &len)) {
      digitalWrite(led, HIGH);

      // Serialize single byte variables into outBuf
      memcpy(&h.recipient, p, sizeof(h.recipient));
      p += sizeof(h.recipient);
      memcpy(&h.sender, p, sizeof(h.sender));
      p += sizeof(h.sender);
      memcpy(&h.hops, p, sizeof(h.hops));
      p += sizeof(h.hops);

      // Serialize our multibyte int into outBuf
      uint16_t host = ntohs(h.last_rssi);
      memcpy(&host, p, sizeof(host));
      p += sizeof(host);

      // Serialize our message into outBuf
      memcpy(&d.message, p, sizeof(d.message));
      p += sizeof(d.message);

      digitalWrite(led, LOW);

      Serial.print(F("Message received: "));
      Serial.println(d.message);

      // Convert headers back to struct and update routing table - or do this as part of routing table update
      //updateRoutingTable(headers);

      // Send an ACK if we have not received an ACK
      if (rf95.headerFlags() != 1) {
        char ACKMessage[] = "ACK";
        sendACK(ACKMessage, sizeof(ACKMessage));
      }
    }
    else
    {
      Serial.println(F("recv function failed"));
    }
  }
}

void waitForACK() {
  if (rf95.waitAvailableTimeout(30000)) {
    checkForMessages();
  }
  else {
    Serial.println(F("No reply"));
  }
}

void updateRoutingTable(uint8_t) {}

void loop() {
  // Enter listen mode
  checkForMessages();

  // If we have a message in our serial buffer, enter sending mode
  String data;
  data = Serial.readString();

  if (data.length() > 0) {
    char message[data.length()];
    data.toCharArray(message, sizeof(message));

    sendMessage(message);
    // Wait for an ACK from our message
    waitForACK();
  }
}
