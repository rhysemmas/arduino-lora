#include "linked_list.h"

linked_list::linked_list() {
  head = NULL;
  tail = NULL;
  count = 0;
}

linked_list::~linked_list() {
  delete head;
  delete tail;
}

void linked_list::add_node(Packet pkt) {
  node *new_node = new node;
  new_node->packet = pkt;
  new_node->prev = NULL;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
    tail = new_node;

    count++;
    return;
  }

  new_node->prev = tail;
  tail->next = new_node;
  tail = new_node;
  count++;
}

void linked_list::delete_node(Packet pkt) {
  node *del_node;
  del_node = head;

  if (head == tail) {
    delete del_node;
    head = NULL;
    tail = NULL;
    count--;
    return;
  }

  // looping problem must be happening here?
  while(del_node->next != nullptr) {
    Serial.println(F("in linked list loop"));
    // messages need IDs or Packet needs isEqual func/operator
    if (del_node->packet.data.message == pkt.data.message) {
      Serial.println(F("found node to delete"));
      node *prev_node;
      prev_node = del_node->prev;
      prev_node->next = del_node->next;

      delete del_node;
      count--;
      return;
    }
    del_node = del_node->next;
  }
}

int linked_list::size() {
  return count;
}

bool linked_list::is_empty() {
  return count == 0;
}

Packet linked_list::peek() {
  return head->packet;
}
