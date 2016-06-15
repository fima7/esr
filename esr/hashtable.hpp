// Copyright 2016
#ifndef ESR_HASHTABLE_FLYMAKE_HPP_
#define ESR_HASHTABLE_FLYMAKE_HPP_

#include <ostream>
#include <iomanip>
#include <utility>
#include <algorithm>

#include <esr/hasher.hpp>
#include <esr/linkedlist.hpp>
#include <esr/assert.hpp>

namespace esr {

template <typename K, typename V>
class Hashtable {
 public:
  class iterator;

  explicit Hashtable(size_t bucket_count_low = m_BucketCountLowDefault,
                     size_t load_factor_bound_low = m_LoadFactorBoundLowDefault,
                     size_t load_factor_bound_up = m_LoadFactorBoundUpDefault);
  Hashtable(const Hashtable& other);
  Hashtable& operator=(Hashtable other);
  virtual ~Hashtable();

  bool add(const K& key, const V& value);
  void remove(const K& key);

  bool set(const K& key, const V& value);
  const V* get(const K& key) const;
  iterator find(const K& key);

  size_t size() { return m_size; }
  size_t load_factor() const { return (m_LoadFactorMax*m_size)/m_bucket_count; }

  template <typename KK, typename VV>
  friend ostream & operator<<(ostream & os, const Hashtable<KK, VV> & ht);

  class iterator {
    // friend void swap(iterator& lhs, iterator& rhs); //C++11
    // void swap(ForwardIterator& other) noexcept
    // {
    //    using std::swap;
    //    swap(itr, other.iter);
    //}

   public:
    explicit iterator(Hashtable* owner,
                      size_t current_bucket_idx,
                      listnode<K, V>* current_bucket_node_ptr) :
        m_owner(owner),
        m_current_bucket_idx(current_bucket_idx),
        m_current_bucket_node_ptr(current_bucket_node_ptr) {}

    iterator& operator++();
    listnode<K, V>& operator*();
    listnode<K, V>* operator->();
    
    bool operator!=(const iterator& rhs);
    bool operator==(const iterator& rhs);

    /*
    K& key() {}
    V& value() {}
    const V& value() const {}
    */

   private:
    Hashtable* m_owner;
    size_t m_current_bucket_idx;
    listnode<K, V>* m_current_bucket_node_ptr;
  };
  iterator begin();
  iterator end();

 private:
  uint64_t m_size;

  hash_function<K> hash;

  size_t m_load_factor_bound_low;
  size_t m_load_factor_bound_up;
  size_t m_bucket_count_low;

  size_t m_bucket_count;
  linkedlist<K, V>* m_buckets;
  // utilization: empty, avg length of occupied
  bool resize(size_t cardinality);

  static const size_t m_LoadFactorMax = 100;  // 100%
  static const size_t m_LoadFactorBoundLowDefault = 49;  // 49%
  static const size_t m_LoadFactorBoundUpDefault = 99;  // 99%
  static const size_t m_BucketCountLowDefault = 8;  // minimum number of buckets
};

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
const size_t Hashtable<K, V>::m_LoadFactorMax;
template <typename K, typename V>
const size_t Hashtable<K, V>::m_LoadFactorBoundUpDefault;
template <typename K, typename V>
const size_t Hashtable<K, V>::m_LoadFactorBoundLowDefault;
template <typename K, typename V>
const size_t Hashtable<K, V>::m_BucketCountLowDefault;

////////////////////////////////////////////////////////////////////////////////
// Constructors, Destructor and Assignments
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
Hashtable<K, V>::Hashtable(size_t bucket_count_low,   // user may set 0
                           size_t load_factor_bound_low,
                           size_t load_factor_bound_up) :
    hash(bucket_count_low),  // user may set 0
    m_size(0),
    // always allocates/deletes m_BucketCountLowDefault lists in copy assignment
    // rather than anoyng user to provide BucketCountLow
    m_buckets(new linkedlist<K, V>[bucket_count_low]),  // user may set 0
    m_bucket_count(bucket_count_low),
    m_bucket_count_low(bucket_count_low),
    m_load_factor_bound_low(load_factor_bound_low),
    m_load_factor_bound_up(load_factor_bound_up) {
}

template <typename K, typename V>
Hashtable<K, V>::Hashtable(const Hashtable& other) :
    m_size(other.m_size),
    hash(other.hash),
    m_load_factor_bound_low(other.m_load_factor_bound_low),
    m_load_factor_bound_up(other.m_load_factor_bound_up),
    m_bucket_count_low(other.m_bucket_count_low),
    m_bucket_count(other.m_bucket_count),
    // null m_bucket_count == 0
    m_buckets(new linkedlist<K, V>[other.m_bucket_count]) {
  for (int i = 0; i < m_bucket_count; ++i)
    m_buckets[i] = other.m_buckets[i];
}

template <typename K, typename V>
Hashtable<K, V>& Hashtable<K, V>::operator=(Hashtable other) {
  std::swap(m_size, other.m_size);
  std::swap(hash, other.hash);  // check function
  std::swap(m_load_factor_bound_low, other.m_load_factor_bound_low);
  std::swap(m_load_factor_bound_up, other.m_load_factor_bound_up);
  std::swap(m_bucket_count_low, other.m_bucket_count_low);
  std::swap(m_bucket_count, other.m_bucket_count);
  std::swap(m_buckets, other.m_buckets);
  return *this;
}

template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {
  delete [] m_buckets;
}

////////////////////////////////////////////////////////////////////////////////
// Iterator
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
typename Hashtable<K, V>::iterator& Hashtable<K, V>::iterator::operator++() {
  m_current_bucket_node_ptr = m_current_bucket_node_ptr->next();
  if (m_current_bucket_node_ptr == nullptr) {  //  end of current bucket
    // find not empty bucket starting from next one
    size_t next_bucket_idx = m_current_bucket_idx + 1;
    // while (next_not_empty_bucket_idx < m_owner->m_bucket_count) {
    for (int i = next_bucket_idx; i < m_owner->m_bucket_count; ++i) {
      linkedlist<K, V>& next_bucket = m_owner->m_buckets[next_bucket_idx];
      if (next_bucket.empty()) {
        ++next_bucket_idx;
      } else {
        break;
      }
    }
    size_t next_not_empty_bucket_idx = next_bucket_idx;

    if (next_not_empty_bucket_idx < m_owner->m_bucket_count) {
      // Next bucket less then bucket count:
      //               set current to found bucket
      //               entry to begin of bucket
      m_current_bucket_idx = next_not_empty_bucket_idx;
      linkedlist<K, V>& bucket = m_owner->m_buckets[m_current_bucket_idx];
      m_current_bucket_node_ptr = bucket.front();
    } else {
      // Next bucket equal or greater than bucket count
      //               set curent bucket to last bucket
      //               set entry to end of last bucket
      m_current_bucket_idx = m_owner->m_bucket_count - 1;
      m_current_bucket_node_ptr = nullptr;
    }
  }
  return *this;
}

template <typename K, typename V>
listnode<K, V>& Hashtable<K, V>::iterator::operator*() {
  // unlikely // exception
  if (m_current_bucket_node_ptr == nullptr) {
    // && m_current_bucket_idx == m_owner->m_bucket_count - 1;
    _assert("dereferncing end iterator",
            __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
  }
  return *m_current_bucket_node_ptr;
}

template <typename K, typename V>
listnode<K, V>* Hashtable<K, V>::iterator::operator->() {
  if (m_current_bucket_node_ptr == nullptr) {
    // && m_current_bucket_idx == m_owner->m_bucket_count - 1;
    _assert("dereferncing end iterator",
            __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
  }
  return m_current_bucket_node_ptr;
}

template <typename K, typename V>
bool Hashtable<K, V>::iterator::operator!=(const iterator& rhs) {
  return (m_current_bucket_idx != rhs.m_current_bucket_idx) ||
      (m_current_bucket_node_ptr != rhs.m_current_bucket_node_ptr);
}

template <typename K, typename V>
bool Hashtable<K, V>::iterator::operator==(const iterator& rhs) {
  return (m_current_bucket_idx == rhs.m_current_bucket_idx) &&
      (m_current_bucket_node_ptr == rhs.m_current_bucket_node_ptr);
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::begin() {
  linkedlist<K, V>* bucket = nullptr;
  size_t first_not_empty_bucket_idx = 0;
  while (first_not_empty_bucket_idx < m_bucket_count) {
    bucket = &m_buckets[first_not_empty_bucket_idx];
    if (bucket->empty())
      ++first_not_empty_bucket_idx;
    else
      break;
  }

  // Empty hashtable, return end() interator
  if (first_not_empty_bucket_idx == m_bucket_count)
    return end();  // iterator(this, m_bucket_count-1, nullptr);

  // Stands at first entry of hashtable
  return iterator(this, first_not_empty_bucket_idx, bucket->front());
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::end() {
  // m_bucket_cout may be 0
  return iterator(this, m_bucket_count-1, nullptr);
}

////////////////////////////////////////////////////////////////////////////////
// Accessors and Modifiers
////////////////////////////////////////////////////////////////////////////////
// returns:
// success true
// fail false: bad hash function, refer to resize()
template <typename K, typename V>
bool Hashtable<K, V>::add(const K& key, const V& value) {
  // Arguable, created with Hashtable(0) probably to avoid
  // alloc dealloc in assignment while copy
  // for usual case use  Hashtable()
  /*
  if (m_buckets == nullptr)
    m_buckets =  new linkedlist[m_BucketCountLowDefault];
  */
  // expand
  size_t factor = load_factor();
  if (factor > m_load_factor_bound_up) {  // unlikely
    if (!resize(2*m_bucket_count))
      return false;
  }
  size_t bucket_idx  = hash(key);
  linkedlist<K, V>& bucket = m_buckets[bucket_idx];
  bool success = bucket.push_back(key, value);
  m_size += success ? 1 : 0;
  return success;
}

template <typename K, typename V>
void Hashtable<K, V>::remove(const K& key) {
  // shrink
  size_t factor = load_factor();
  if (factor < m_load_factor_bound_low) {  // 0.4 // unlikely
    size_t shrink_size = m_bucket_count/2;
    if (shrink_size >=  m_bucket_count_low)
      if (!resize(m_bucket_count/2))
        return;
  }
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return;
  }
  bool success = m_buckets[bucket_idx].erase(key);
  m_size -= (success ? 1 : 0);
}


template <typename K, typename V>
bool Hashtable<K, V>::set(const K& key, const V& value) {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return false;
  }
  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return false;

  node->set(value);
  return true;
}

template <typename K, typename V>
const V* Hashtable<K, V>::get(const K& key) const {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return nullptr;
  }
  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return nullptr;

  return &node->value();
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::find(const K& key) {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception replaces following line
    return iterator(this, m_bucket_count-1, nullptr);  // end();
  }
  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return iterator(this, m_bucket_count-1, nullptr);

  return iterator(this, bucket_idx, node);
}

////////////////////////////////////////////////////////////////////////////////
// Resize
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
bool Hashtable<K, V>::resize(size_t bucket_count) {
  // std::cerr << "resize to: " << bucket_count << '\n';
  // reasign current iterator
  // no, add with iteration
  std::unique_ptr<linkedlist<K, V>[]> ptr(new linkedlist<K, V>[bucket_count]);
  linkedlist<K, V>* table = ptr.get();
  hash = hash_function<K>(bucket_count);  // new hash function from family

  // Rehash: add all entries of old table to new one
  for (int i = 0; i < m_bucket_count; ++i) {
    linkedlist<K, V>& bucket = m_buckets[i];
    for (listnode<K, V>* node = bucket.front(); node; node = node->next()) {
      size_t bucket_idx = hash(node->key());
      if (bucket_idx < 0 || bucket_idx >= bucket_count) {
        // excpt. wrong hash index: public out_of_range
        return false;
      }
      if (!table[bucket_idx].push_back(node->key(), node->value()))
        return false;
    }
  }
  ptr.release();

  delete [] m_buckets;
  m_bucket_count = bucket_count;
  m_buckets = table;

  // m_size successfully added

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Printout
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
ostream & operator<<(ostream & os, const Hashtable<K, V> & htable) {
  for (int i = 0; i < htable.m_bucket_count; ++i)
    os << std::setw(3) << i << ": {" << htable.m_buckets[i] << "}\n";
  return os;
}

}  // namespace esr

#endif  // ESR_HASHTABLE_FLYMAKE_HPP_
