// Copyright 2016
#ifndef ESR_LINKEDLIST_FLYMAKE_HPP_
#define ESR_LINKEDLIST_FLYMAKE_HPP_
////////////////////////////////////////////////////////////////////////////////
// Linked List <K, V>.
////////////////////////////////////////////////////////////////////////////////
#include <algorithm>  // std::swap()
#include <ostream>    // operator<<()
using std::ostream;

namespace esr {

////////////////////////////////////////////////////////////////////////////////
/// @class listnode.
///
/// @brief Node of Linked List.
/// Node contains key and value.
/// @tparam K type of hash key.
/// @tparam V type of hash value.
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
class listnode {
  template <typename KK, typename VV>
  friend class linkedlist;
 public:
  /// Default constructor, creates empty node without link.
  listnode() : m_next(nullptr) {}

  /// @brief Constructor for node.
  /// Creates node with key, value without link.
  listnode(const K& key, const V& value) :
      m_key(key), m_value(value), m_next(nullptr) {}

  /// @brief Constructor for node.
  /// Creates node with key, value with link to next node.
  listnode(const K& key, const V& value, listnode* next) :
      m_key(key), m_value(value), m_next(next) {}

  // deprecated: K& key() { return m_key; }

  /// @brief Gets key.
  /// Returns reference to key.
  const K& key() const { return m_key; }

  /// @brief Gets mutable value.
  /// Returns reference to value.
  V& value() { return m_value; }

  /// @brief Gets immutable value.
  /// Returns reference to value.
  const V& value() const { return m_value; }

  /// @brief Sets the value.
  void set(const V& value) { m_value = value; }

  /// @brief Gets next node.
  /// Returns pointer to the next to this node.
  listnode* next() { return m_next; }

  /// @brief Printout node
  friend ostream & operator<<(ostream & os, const listnode<K, V>& node) {
    return os << node.m_key <<"=>"<< node.m_value;
  }

 private:
  K m_key;
  V m_value;
  listnode *m_next;
};

////////////////////////////////////////////////////////////////////////////////
/// @class linkedlist.
///
/// @brief Linked List.
/// List to hold the chain of key value nodes.
/// @tparam K type of hash key.
/// @tparam V type of hash value.
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
class linkedlist {
 public:
  /// Default constructor.
  linkedlist();

  /// Copy constructor.
  linkedlist(const linkedlist& other);

  /// Destructor.
  ~linkedlist();

  /// Assignmet operator.
  linkedlist& operator=(linkedlist other);

  /// Adds element to List.
  bool push_back(const K& key, const V& value);

  /// Gets immutable element from List by it's key.
  const listnode<K, V>* find(const K& key) const;

  /// Gets mutable element from List by it's key.
  listnode<K, V>* find(const K& key);

  /// Gets first element of List.
  listnode<K, V>* front();

  /// Removes element from List.
  bool erase(const K& key);

  /// Gets a number of elements in List.
  size_t size() { return m_size; }

  /// Is List empty or not.
  bool empty();

  /// Printout List.
  template <typename KK, typename VV>
  friend ostream & operator<<(ostream & os, const linkedlist<KK, VV>& ll);

 private:
  size_t m_size;            //< Number of elements of Linked List.
  listnode<K, V> *m_front;  //< First element of Linked List.
  listnode<K, V> *m_back;   //< Last element of Linked List.
  void clear();
};

////////////////////////////////////////////////////////////////////////////////
// Constructors, Destructor and Assignment.
////////////////////////////////////////////////////////////////////////////////

/// @brief Default constructor.
/// Creates an empty List.
/// @tparam K type of key.
/// @tparam V type of value.
template <typename K, typename V>
linkedlist<K, V>::linkedlist() : m_size(0), m_front(nullptr), m_back(nullptr) {}

/// @brief Copy constructor.
/// Creates a copy of List instance.
/// @tparam K type of key.
/// @tparam V type of value.
template <typename K, typename V>
linkedlist<K, V>::linkedlist(const linkedlist& other) :
    m_size(0), m_front(nullptr), m_back(nullptr) {
  for (listnode<K, V>* node = other.m_front; node; node = node->m_next)
    push_back(node->m_key, node->m_value);
}

/// @brief Destructor for List.
/// Removes content of List instance.
/// @tparam K type of key.
/// @tparam V type of value.
template <typename K, typename V>
linkedlist<K, V>::~linkedlist() {
  clear();
}

/// @brief Assignment operator for List.
/// Creates copy of existing List instance,
/// cleaning up left-hand target.
/// @tparam K type of key.
/// @tparam V type of value.
template <typename K, typename V>
linkedlist<K, V>& linkedlist<K, V>::operator=(linkedlist other) {
  std::swap(m_size, other.m_size);
  std::swap(m_front, other.m_front);
  std::swap(m_back, other.m_back);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Accessors and Modifiers.
////////////////////////////////////////////////////////////////////////////////

/// @brief Adds an element.
/// Adds an element to List using it's key, value.
/// Traverses a list comparing keys to find dublicate key.
/// Creates new element with key, value and appends it
/// to back of List, if no duplicate key found.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return result of adding.
/// @retval true key has been added successfully.
/// @retval false duplicate key found.
template <typename K, typename V>
bool linkedlist<K, V>::push_back(const K& key, const V& value) {
  if (m_front == nullptr) {
    m_back = m_front = new listnode<K, V>(key, value);
  } else {
    // issue: no need to have a tail because of that.
    for (listnode<K, V>* node = m_front; node; node = node->m_next)
      if (node->m_key == key) {
        return false;  // dublicate keys
      }
    m_back->m_next = new listnode<K, V>(key, value);
    m_back = m_back->m_next;
  }
  m_size++;
  return true;
}

/// @brief Removes an element.
/// Removes an element from List by it's key.
/// Traverses a list comparing keys to find a matched key,
/// removes found element from List.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return result of removal.
/// @retval true key has been removed successfully.
/// @retval false no key found.
template <typename K, typename V>
bool linkedlist<K, V>::erase(const K& key) {
  listnode<K, V>* node;
  listnode<K, V>* prev = nullptr;
  for ( node = m_front; node; node = node->m_next ) {
    if ( node->m_key == key ) {
      if ( node == m_front ) {
        m_front = node->m_next;
        if (node == m_back)
          m_back = nullptr;
        delete node;
        m_size--;
        return true;
      } else {
        prev->m_next = node->m_next;
        if (node == m_back)
          m_back = prev;
        delete node;
        m_size--;
        return true;
      }
    }
    prev = node;
  }
  return false;
}

/// @brief Finds an element, immutable.
/// Returns found element by it's key.
/// Traverses a list comparing keys to find a matched key,
/// @tparam K type of key.
/// @tparam V type of value.
/// @return immutable list node.
/// @retval true key has been removed successfully.
/// @retval false no key found.
template <typename K, typename V>
const listnode<K, V>* linkedlist<K, V>::find(const K& key) const {
  for (listnode<K, V>* node = m_front; node; node = node->m_next)
    if (node->m_key == key)
      return node;
    // likely, because list size is usually about 1
  return nullptr;
}

/// @brief Finds an element, mutable.
/// Returns found element by it's key.
/// Traverses a list comparing keys to find a matched key,
/// @tparam K type of key.
/// @tparam V type of value.
/// @return immutable list node.
/// @retval pointer to node, if element found.
/// @retval nullptr, if element is not found.
template <typename K, typename V>
listnode<K, V>* linkedlist<K, V>::find(const K& key) {
  for (listnode<K, V>* node = m_front; node; node = node->m_next)
    if (node->m_key == key)
      return node;
    // likely, because list size is usually about 1
  return nullptr;
}

/// @brief Gets first element.
/// Returns first element.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return found element.
/// @retval pointer to found node
/// @retval nullptr otherwise
template <typename K, typename V>
listnode<K, V>* linkedlist<K, V>::front() {
  return m_front;
}

/// @brief Cheks whether list empty of not.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return list empy or not.
/// @retval true if List is empty.
/// @retval false otherwise.
template <typename K, typename V>
bool linkedlist<K, V>::empty() {
  if (m_front == nullptr)
    return true;
  return false;
}

/// @brief Removes all elements from List.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return nothing.
template <typename K, typename V>
void linkedlist<K, V>::clear() {
  listnode<K, V> *node_to_delete = m_front;
    while (node_to_delete != nullptr) {
        m_front = m_front->m_next;
        delete node_to_delete;
        node_to_delete = m_front;
    }
    m_size = 0;
    m_back = m_front = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Printout
////////////////////////////////////////////////////////////////////////////////

/// @brief Printout List.
/// @tparam K type of key.
/// @tparam V type of value.
/// @return nothing.
template <typename K, typename V>
ostream & operator<<(ostream & os, const linkedlist<K, V>& ll) {
  for (listnode<K, V> *node = ll.m_front; node != nullptr; node = node->next())
    os << '(' << *node << ')';
  return os;
}

}  // namespace esr

#endif  // ESR_LINKEDLIST_FLYMAKE_HPP_
