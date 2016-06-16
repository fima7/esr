// Copyright 2016
#ifndef ESR_HASHTABLE_FLYMAKE_HPP_
#define ESR_HASHTABLE_FLYMAKE_HPP_

#include <ostream>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <cassert>

#include <esr/hasher.hpp>
#include <esr/linkedlist.hpp>
#include <esr/hashexcept.hpp>

namespace esr {

template <typename K, typename V>
class Hashtable {
 public:
  class iterator;

  explicit Hashtable(size_t load_factor_bound_low = m_LoadFactorBoundLowDefault,
                     size_t load_factor_bound_up = m_LoadFactorBoundUpDefault);
  Hashtable(const Hashtable& other);
  Hashtable& operator=(Hashtable other);
  virtual ~Hashtable();

  bool add(const K& key, const V& value);
  void remove(const K& key);

  bool set(const K& key, const V& value);
  const V* get(const K& key) const;
  iterator find(const K& key);

  size_t size() const { return m_size; }

  // user may set 0
  size_t load_factor() const {
    return (
        (m_bucket_count == 0) ?
        0 : (m_LoadFactor100Percents*m_size)/m_bucket_count);
  }

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

  size_t m_bucket_count;
  linkedlist<K, V>* m_buckets;
  // utilization: empty, avg length of occupied
  void resize(size_t cardinality);

  static const size_t m_LoadFactor100Percents = 100;  // size == buckets, 100%
  static const size_t m_LoadFactorBoundLowDefault = 49;  // 49%
  static const size_t m_LoadFactorBoundUpDefault = 99;  // 99%
  static const size_t m_BucketCountLowDefault = 8;  // minimum number of buckets
};

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
const size_t Hashtable<K, V>::m_LoadFactor100Percents;
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
Hashtable<K, V>::Hashtable(size_t load_factor_bound_low,
                           size_t load_factor_bound_up) :
    hash(0),
    m_size(0),
    m_bucket_count(0),
    m_buckets(nullptr),
    m_load_factor_bound_low(load_factor_bound_low),
    m_load_factor_bound_up(load_factor_bound_up) {
}

template <typename K, typename V>
Hashtable<K, V>::Hashtable(const Hashtable& other) :
    m_size(other.m_size),
    hash(other.hash),
    m_load_factor_bound_low(other.m_load_factor_bound_low),
    m_load_factor_bound_up(other.m_load_factor_bound_up),
    m_bucket_count(other.m_bucket_count),
    m_buckets(
        (other.m_bucket_count > 0) ?
        (new linkedlist<K, V>[other.m_bucket_count]) : (nullptr)) {
  for (int i = 0; i < m_bucket_count; ++i)
    m_buckets[i] = other.m_buckets[i];
}

template <typename K, typename V>
Hashtable<K, V>& Hashtable<K, V>::operator=(Hashtable other) {
  std::swap(m_size, other.m_size);
  std::swap(hash, other.hash);  // check function
  std::swap(m_load_factor_bound_low, other.m_load_factor_bound_low);
  std::swap(m_load_factor_bound_up, other.m_load_factor_bound_up);
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
    }
  }
  return *this;
}

template <typename K, typename V>
listnode<K, V>& Hashtable<K, V>::iterator::operator*() {
  // unlikely // exception
  if (m_current_bucket_node_ptr == nullptr) {
    assert(m_current_bucket_idx == m_owner->m_bucket_count - 1);
    throw exception::end_iterator(m_current_bucket_idx,
                                  __ESR_PRETTY_FUNCTION__);
  }
  return *m_current_bucket_node_ptr;
}

template <typename K, typename V>
listnode<K, V>* Hashtable<K, V>::iterator::operator->() {
  if (m_current_bucket_node_ptr == nullptr) {
    assert(m_current_bucket_idx == m_owner->m_bucket_count - 1);
    throw exception::end_iterator(m_current_bucket_idx,
                                  __ESR_PRETTY_FUNCTION__);
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
  assert(first_not_empty_bucket_idx <= m_bucket_count);
  // Empty hashtable, return end() interator
  if (first_not_empty_bucket_idx == m_bucket_count)
    return iterator(this, m_bucket_count-1, nullptr);
  // don't want to return end(), because of following line

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
template <typename K, typename V>
bool Hashtable<K, V>::set(const K& key, const V& value) {
  if (m_size == 0) {
    assert(((m_buckets == nullptr) &&  (m_bucket_count == 0)));
    return false;  // end() iterator
  }

  size_t bucket_idx = hash(key);
  if (bucket_idx >= m_bucket_count)
    throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);

  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return false;

  node->set(value);
  return true;
}

template <typename K, typename V>
const V* Hashtable<K, V>::get(const K& key) const {
  if (m_size == 0) {
    assert(((m_buckets == nullptr) &&  (m_bucket_count == 0)));
    return nullptr;  // end() iterator
  }

  size_t bucket_idx = hash(key);
  if (bucket_idx >= m_bucket_count)
    throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);

  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return nullptr;

  return &node->value();
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::find(const K& key) {
  if (m_size == 0) {
    assert(((m_buckets == nullptr) &&  (m_bucket_count == 0)));
    return iterator(this, m_bucket_count-1, nullptr);  // end() iterator
  }

  size_t bucket_idx = hash(key);
  if (bucket_idx >= m_bucket_count)
    throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);

  listnode<K, V>* node = m_buckets[bucket_idx].find(key);
  if (node == nullptr)
    return iterator(this, m_bucket_count-1, nullptr);

  return iterator(this, bucket_idx, node);
}

////////////////////////////////////////////////////////////////////////////////
// Isertions, Deletion, Resize
////////////////////////////////////////////////////////////////////////////////
// returns:
// success true
// fail false: bad hash function, refer to resize()
template <typename K, typename V>
bool Hashtable<K, V>::add(const K& key, const V& value) {
  // first element
  if (m_size == 0)
    resize(1/*m_BucketCountLowDefault*/);

  // expand
  size_t factor = load_factor();
  if (factor > m_load_factor_bound_up)  // unlikely
    resize(2*m_bucket_count);

  size_t bucket_idx  = hash(key);
  if (bucket_idx >= m_bucket_count)
    throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);

  linkedlist<K, V>& bucket = m_buckets[bucket_idx];
  bool success = bucket.push_back(key, value);
  m_size += success ? 1 : 0;

  return success;
}

template <typename K, typename V>
void Hashtable<K, V>::remove(const K& key) {
  if (m_size == 0) {
    assert(((m_buckets == nullptr) &&  (m_bucket_count == 0)));
    return;
  }

  size_t bucket_idx = hash(key);
  if (bucket_idx >= m_bucket_count)
    throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);

  bool success = m_buckets[bucket_idx].erase(key);
  if (!success) return;  // no such key

  --m_size;

  // shrink
  size_t factor = load_factor();
  if (factor == 0) {
    resize(0);
    return;
  }
  if (factor < m_load_factor_bound_low) {  // unlikely
    size_t shrunk_bucket_count = m_bucket_count/2;
    resize(shrunk_bucket_count);
  }
}

template <typename K, typename V>
void Hashtable<K, V>::resize(size_t bucket_count) {
  assert(bucket_count != m_bucket_count);
  size_t size = 0;
  linkedlist<K, V>* table = nullptr;
  if (bucket_count != 0) {
    std::unique_ptr<linkedlist<K, V>[]> ptr(new linkedlist<K, V>[bucket_count]);
    table = ptr.get();
    hash = hash_function<K>(bucket_count);  // new hash function from family

    // Rehash: add all entries of old table to new one
    for (int i = 0; i < m_bucket_count; ++i) {
      linkedlist<K, V>& bucket = m_buckets[i];
      for (listnode<K, V>* node = bucket.front(); node; node = node->next()) {
        size_t bucket_idx =  hash(node->key());
        if (bucket_idx < 0 || bucket_idx >= bucket_count)
          throw exception::bucket_index(bucket_idx, __ESR_PRETTY_FUNCTION__);
        bool success = table[bucket_idx].push_back(node->key(), node->value());
        assert(success);
        ++size;
      }
    }
    ptr.release();
    if (m_buckets != nullptr)
      delete [] m_buckets;
  }
  m_size = size;
  m_bucket_count = bucket_count;
  m_buckets = table;
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
