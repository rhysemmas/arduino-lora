#include <queue.h>

class handle {
  RH_RF95 rf95;
  queue *in;
  queue *out;

public:
    handle(RH_RF95 driver);
    ~handle();

    bool receive();
    void read();
    bool schedule(Packet pkt);
    void send();
};
