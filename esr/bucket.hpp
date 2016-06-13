// Copyright 2016
#ifndef ESR_ESR_HASHLIST_FLYMAKE_HPP_
#define ESR_ESR_HASHLIST_FLYMAKE_HPP_

#include <ostream>
#include <utility>

// noexcept
#include <esr/assert.hpp>

namespace esr {

using std::ostream;
using std::pair;

template <typename K, typename V>
class bucket {
  class entry;
 public:
  bucket() : m_head(nullptr), m_size(0) {}

  size_t size();
  
  const V* get(const K & key) const;
  bool set(const K& key, const V& value);
  
  bool add(const K& key, const V& value);
  bool rm(const K& key);

  entry* get_node(const K & key) const;
  bool empty() { return (m_head == nullptr) ? true : false; }

  template <typename KK, typename VV>
  friend ostream & operator<<(ostream & os, const bucket<KK, VV> & lst);

  class iterator {
   public:
    // iterator() : m_current(nullptr) {}
    explicit iterator(entry* current) : m_current(current) {}
    pair<K&, V&> operator*() {
      if (m_current == nullptr) {  // ulikely
        // exception
        _assert("dereferncing end iterator",
                __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
      }
      return pair<K&, V&>(m_current->key, m_current->value);
    }
    iterator& operator++() {
      if (m_current != nullptr)
        m_current = m_current->next;
      return *this;
    }

    // performance ?
    K& key() {
      // exception
      if (m_current == nullptr) {
       // exception
        _assert("access data by end iterator",
                __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
      }
      return m_current->key;
    }
    V& value() {
      if (m_current == nullptr) {
        // exception
        _assert("access data by end iterator",
                __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
      }
      return m_current->value;
    }

    bool operator!=(const iterator& rhs) {
      return m_current != rhs.m_current;
    }

    bool operator==(const iterator& rhs) {
      return m_current == rhs.m_current;
    }

   private:
    entry* m_current;
  };
  iterator begin() { return iterator(m_head); }
  iterator end() { return iterator(nullptr); }

 private:
  struct entry {
    K key;
    V value;
    entry* next;
  };
  entry* m_head;
  size_t m_size;  // number of elements in bucket
};
// list

// custom iterator too expensive
// (source code and empty element at the end of the list)
template <typename K, typename V>
const V* bucket<K, V>::get(const K & key) const {
  for (entry* n = m_head; n; n = n->next)
    if (key == n->key) return &n->value;  // unlikely
  return nullptr;
}

template <typename K, typename V>
typename bucket<K, V>::entry* bucket<K, V>::get_node(const K & key) const {
  for (entry* n = m_head; n; n = n->next)
    if (key == n->key) return n;  // unlikely
  return nullptr;
}

template <typename K, typename V>
bool bucket<K, V>::add(const K& key, const V& value) {
  // _assert1(1);
  entry* nn = new entry;
  nn->key = key;
  nn->value = value;
  nn->next = nullptr;
  if ( m_head == nullptr ) {
    m_head = nn;
    return true;
  }
  entry* last;
  for (entry* n = m_head; n; n = n->next) {
    if (n->key == key) return false;
    last = n;
  }
  last->next = nn;
  return true;
}

template <typename K, typename V>
bool bucket<K, V>::set(const K& key, const V& value) {
  for (entry* n = m_head; n; n = n->next)
    // unlikely, no likely as long as load factor <= 1
    if (key == n->key) {
      n->value = value;
      return true;
    }
  return false;
}

template <typename K, typename V>
bool bucket<K, V>::rm(const K& key) {
  struct entry *n, *prev;
  for ( n = m_head; n; n = prev = n, n->next ) {
    if ( n->key == key ) {
      if ( n == m_head ) {
        m_head = n->next;
        delete n;
        return true;
      } else {
        prev->next = n->next;
        delete n;
        return true;
      }
    }
  }
  return false;
}

template <typename K, typename V>
ostream & operator<<(ostream & os, const bucket<K, V> & lst) {
  if ( !lst.m_head ) {
    os << "(null)\n";
    return os;
  }
  for (auto* n = lst.m_head; n; n = n->next)
    os << "[" << n->key << "=>" << n->value << "] ";
  os << '\n';
  return os;
}

}  // namespace esr

#endif  // ESR_ESR_HASHLIST_FLYMAKE_HPP_
