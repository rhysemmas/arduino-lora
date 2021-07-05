#include <linked_list.h>

class handle {
  private:
    RH_RF95 *rf95;
    linked_list *in;
    linked_list *out;

  public:
    handle();
    ~handle();

    void receive();
    void send();
    bool read();
    bool schedule(Packet pkt);
};
