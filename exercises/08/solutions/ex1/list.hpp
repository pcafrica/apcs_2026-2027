#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"

// Doubly linked list. Owns its nodes; pointers returned by find are borrowed.
class List {
public:
  List() : head(nullptr) {}

  List(const List &) = delete;
  List &operator=(const List &) = delete;

  ~List() {
    if (head != nullptr)
      delete head;
  }

  // Requires a non-null, heap-allocated, unlinked node. Transfers ownership.
  void append(Node *new_node);

  void append(int value);

  // Requires a node belonging to this list, or nullptr. Invalidates that node.
  void erase(Node *node);

  void erase(int value);

  // Borrowed pointer, valid until that node is erased or this list is
  // destroyed.
  Node *find(int value) const;

  void print() const;

private:
  Node *head;
};

#endif /* LIST_HPP */
