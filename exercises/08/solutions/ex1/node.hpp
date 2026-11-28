#ifndef NODE_HPP
#define NODE_HPP

// Owns the chain reached through next; previous is a borrowed link.
class Node {
public:
  Node() : next(nullptr), previous(nullptr), data(0) {}

  Node(int value) : next(nullptr), previous(nullptr), data(value) {}

  Node(const Node &) = delete;
  Node &operator=(const Node &) = delete;

  ~Node() {
    if (next != nullptr)
      delete next;
  }

  void set_data(int value) { data = value; }

  int get_data() const { return data; }

  void set_next(Node *next) { this->next = next; }

  Node *get_next() const { return next; }

  void set_previous(Node *previous) { this->previous = previous; }

  Node *get_previous() const { return previous; }

  bool is_first() const { return (previous == nullptr); }

  bool is_last() const { return (next == nullptr); }

protected:
  Node *next;
  Node *previous;

private:
  int data;
};

#endif /* NODE_HPP */
