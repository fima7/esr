// Copyright 2016
#include <iostream>
#include <ostream>
#include <utility>
#include <algorithm>

using std::ostream;

////////////////////////////////////////////////////////////////////////////////
// Linked List Implementetion
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
class listnode {
  template <typename KK, typename VV>
  friend class linkedlist;
 public:
  listnode(void) : m_next(nullptr) {}
  explicit listnode(const K& key, const V& value) :
      m_key(key), m_value(value), m_next(nullptr) {}
  listnode(const K& key, const V& value, listnode* next) :
      m_key(key), m_value(value), m_next(next) {}

  // deprecated: K& key() { return m_key; }
  const K& key() const { return m_key; }

  V& value() { return m_value; }
  const V& value() const { return m_value; }

  listnode* next() { return m_next; }

  friend ostream & operator<<(ostream & os, const listnode<K, V>& node) {
    return os << node.m_key <<"=>"<< node.m_value;
  }

 private:
  K m_key;
  V m_value;
  listnode *m_next;
};

template <typename K, typename V>
class linkedlist {
 public:
  linkedlist();
  linkedlist(const linkedlist& other);
  ~linkedlist();

  linkedlist & operator=(linkedlist other);

  void push_back(const K& key, const V& value);
  const listnode<K, V>* find(const K& key) const;
  listnode<K, V>* find(const K& key);
  bool erase(const K& key);
  size_t size() { return m_size; }

  template <typename KK, typename VV>
  friend ostream & operator<<(ostream & os, const linkedlist<KK, VV>& ll);

 private:
  size_t m_size;
  listnode<K, V> *m_head;
  listnode<K, V> *m_tail;
  void clear();
};

////////////////////////////////////////////////////////////////////////////////
// Constructors, destructor and assignments
////////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
linkedlist<K, V>::linkedlist() : m_size(0), m_head(nullptr), m_tail(nullptr) {}

template <typename K, typename V>
linkedlist<K, V>::linkedlist(const linkedlist& other) :
    m_size(0), m_head(nullptr), m_tail(nullptr) {
  for (listnode<K, V>* node = other.m_head; node; node = node->m_next)
    push_back(node->m_key, node->m_value);
}

template <typename K, typename V>
linkedlist<K, V>::~linkedlist() {
  clear();
}

template <typename K, typename V>
linkedlist<K, V>& linkedlist<K, V>::operator=(linkedlist other) {
  std::swap(m_size, other.m_size);
  std::swap(m_head, other.m_head);
  std::swap(m_tail, other.m_tail);
  return *this;
}

template <typename K, typename V>
void linkedlist<K, V>::push_back(const K& key, const V& value) {
  if (m_head == nullptr) {
    m_tail = m_head = new listnode<K, V>(key, value);
  } else {
    m_tail->m_next = new listnode<K, V>(key, value);
    m_tail = m_tail->m_next;
  }
  m_size++;
}

template <typename K, typename V>
bool linkedlist<K, V>::erase(const K& key) {
  listnode<K, V>* node;
  listnode<K, V>* prev = nullptr;
  for ( node = m_head; node; node = node->m_next ) {
    if ( node->m_key == key ) {
      if ( node == m_head ) {
        m_head = node->m_next;
        if (node == m_tail)
          m_tail = nullptr;
        delete node;
        m_size--;
        return true;
      } else {
        prev->m_next = node->m_next;
        if (node == m_tail)
          m_tail = prev;
        delete node;
        m_size--;
        return true;
      }
    }
    prev = node;
  }
  return false;
}

template <typename K, typename V>
const listnode<K, V>* linkedlist<K, V>::find(const K& key) const {
  for (listnode<K, V>* node = m_head; node; node = node->m_next)
    if (node->m_key == key)
      return node;
    // likely, because list size is usually about 1
  return nullptr;
}

template <typename K, typename V>
listnode<K, V>* linkedlist<K, V>::find(const K& key) {
  for (listnode<K, V>* node = m_head; node; node = node->m_next)
    if (node->m_key == key)
      return node;
    // likely, because list size is usually about 1
  return nullptr;
  // return const_cast<V*> ((*this).find(key));
}

template <typename K, typename V>
void linkedlist<K, V>::clear() {
  listnode<K, V> *node_to_delete = m_head;
    while (node_to_delete != nullptr) {
        m_head = m_head->m_next;
        delete node_to_delete;
        node_to_delete = m_head;
    }
    m_size = 0;
    m_tail = m_head = nullptr;
}

template <typename K, typename V>
ostream & operator<<(ostream & os, const linkedlist<K, V>& ll) {
  listnode<K, V> *node = ll.m_head;
  if (ll.m_head == nullptr) {
    os << "empty";;
    return os;
  }
  while (node != nullptr) {
    os << '(' << *node << ')' << " ";
    node = node->next();
  }
  return os;
}

