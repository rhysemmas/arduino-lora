#include <packet.h>

struct node {
  node *next;
  Packet packet;
  node *prev;
};

class linked_list {
  private:
    node *head;
    node *tail;
    int count;

  public:
    linked_list();
    ~linked_list();

    void add_node(Packet pkt);
    void delete_node(Packet pkt);
    int size();
    bool is_empty();
    Packet peek();
};
