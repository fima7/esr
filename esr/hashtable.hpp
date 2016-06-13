// Copyright 2016
#ifndef ESR_ESR_HASHTABLE_FLYMAKE_HPP_
#define ESR_ESR_HASHTABLE_FLYMAKE_HPP_

#include <ostream>
#include <iomanip>
#include <utility>

// noexcept
#include <esr/assert.hpp>

#include <esr/hasher.hpp>
#include <esr/bucket.hpp>

// typename esr::Hashtable<K, V>::iterator;

namespace esr {

using std::ostream;
using std::pair;

template <typename K, typename V>
class Hashtable {
 public:
  class iterator;
  // Hashtable(const Hashtable& other) {}

  // Hashtable() {}
  // Hashtable(Hashtable&& other) {}
  // Hashtable & operator=(const Hashtable& rhs) { return rhs; }
  // const Hashtable & operator=(const Hashtable& rhs) { return rhs; }
  // Hashtable & operator=(Hashtable& rhs) { return rhs; }


  
  explicit Hashtable(size_t load_factor_bound_low = 49,
                     size_t load_factor_bound_up = 99);
  virtual ~Hashtable();

  bool add(const K& key, const V& value);
  void remove(const K& key);

  bool set(const K& key, const V& value);
  const V* get(const K& key) const;
  const V& get(const K& key, bool *success) const;
  iterator find(const K& key);

  size_t load_factor() const { return (m_MaxLoadFactor*m_size)/m_bucket_count; }

  template <typename KK, typename VV>
  friend ostream & operator<<(ostream & os, const Hashtable<KK, VV> & ht);

  class iterator {
   public:
    /*
    iterator() :
        m_owner(nullptr),
        m_current_bucket_idx(0) {}
    */
    typedef typename bucket<K, V>::iterator bucket_iterator_t;
    explicit iterator(Hashtable* owner,
                      size_t current_bucket_idx,
                      const bucket_iterator_t& current_entry_it) :
        m_owner(owner),
        m_current_bucket_idx(current_bucket_idx),
        m_current_entry_it(current_entry_it) {}

    iterator& operator++();
    pair<K&, V&> operator*();
    /*
    pair<K&, V&>* operator->() {
      // unlikely // exception
      if (m_current_entry_it ==
          m_owner->m_buckets[m_owner->m_bucket_count-1].end()) {
        _assert("dereferncing end iterator",
                __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
      }
      K k = 1;
      V v = 1;
      pair<K&, V&> pr(k, v);
      return &pr;
      return &(*m_current_entry_it);
    }
    */
    bool operator!=(const iterator& rhs);
    bool operator==(const iterator& rhs);

    K& key() { return m_current_entry_it.key(); }
    V& value() { return m_current_entry_it.value(); }

   private:
    Hashtable* m_owner;
    size_t m_current_bucket_idx;
    typename bucket<K, V>::iterator m_current_entry_it;
  };
  iterator begin();
  iterator end();

 private:
  static const size_t m_MaxLoadFactor = 100;  // 100%
  size_t m_load_factor_bound_low;
  size_t m_load_factor_bound_up;

  bucket<K, V>* m_buckets;
  size_t m_bucket_count;
  uint64_t m_size;

  hash_function<K> hash;

  bool resize(size_t cardinality);
};

////////////////////////////////////////////////////////////////////////////////
// Iterator
////////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
typename Hashtable<K, V>::iterator& Hashtable<K, V>::iterator::operator++() {
  ++m_current_entry_it;

  if (m_current_entry_it == m_owner->m_buckets[m_current_bucket_idx].end()) {
    // end of bucket: find not empty bucket starting from next one
    size_t next_not_empty_bucket_idx = m_current_bucket_idx + 1;
    while (next_not_empty_bucket_idx < m_owner->m_bucket_count) {
      if (m_owner->m_buckets[next_not_empty_bucket_idx].empty()) {
        ++next_not_empty_bucket_idx;
      } else {
        break;
      }
    }
    // std::cerr<< "Next bucket: " << next_not_empty_bucket_idx << '\n';
    if (next_not_empty_bucket_idx < m_owner->m_bucket_count) {
      // Next bucket less then bucket count:
      //               set current to found bucket
      //               entry to begin of bucket
      m_current_bucket_idx = next_not_empty_bucket_idx;
      m_current_entry_it = m_owner->m_buckets[m_current_bucket_idx].begin();
    } else {
      // Next bucket equal or greater than bucket count
      //               set curent bucket to last bucket
      //               set entry to end of last bucket
      m_current_bucket_idx = m_owner->m_bucket_count - 1;
      m_current_entry_it = m_owner->m_buckets[m_current_bucket_idx].end();
    }
  }
  return *this;
}

template <typename K, typename V>
pair<K&, V&> Hashtable<K, V>::iterator::operator*() {
  // unlikely // exception
  if (m_current_entry_it ==
      m_owner->m_buckets[m_owner->m_bucket_count-1].end()) {
    _assert("dereferncing end iterator",
            __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
  }
  return *m_current_entry_it;
}

template <typename K, typename V>
bool Hashtable<K, V>::iterator::operator!=(const iterator& rhs) {
  return (m_current_bucket_idx != rhs.m_current_bucket_idx) ||
      (m_current_entry_it != rhs.m_current_entry_it);
}

template <typename K, typename V>
bool Hashtable<K, V>::iterator::operator==(const iterator& rhs) {
  return (m_current_bucket_idx == rhs.m_current_bucket_idx) &&
      (m_current_entry_it == rhs.m_current_entry_it);
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::begin() {
    size_t first_not_empty_bucket_idx = 0;
    while (first_not_empty_bucket_idx < m_bucket_count) {
      if (m_buckets[first_not_empty_bucket_idx].empty())
        ++first_not_empty_bucket_idx;
      else
        break;
    }

    // Empty hashtable, return end() interator
    if (first_not_empty_bucket_idx == m_bucket_count)
      return iterator(this, m_bucket_count-1,
                      m_buckets[m_bucket_count-1].end());

    // Stands at first entry of hashtable
    return iterator(this, first_not_empty_bucket_idx,
                    m_buckets[first_not_empty_bucket_idx].begin());
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::end() {
  return iterator(this, m_bucket_count-1, m_buckets[m_bucket_count-1].end());
}

////////////////////////////////////////////////////////////////////////////////
template <typename K, typename V>
const size_t Hashtable<K, V>::m_MaxLoadFactor;

template <typename K, typename V>
Hashtable<K, V>::Hashtable(size_t load_factor_bound_low,
                           size_t load_factor_bound_up) :
    hash(4),
    m_size(0),
    m_bucket_count(4),
    m_load_factor_bound_low(load_factor_bound_low),
    m_load_factor_bound_up(load_factor_bound_up) {
  m_buckets = new bucket<K, V>[m_bucket_count];
}

template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {
  delete [] m_buckets;
}


template <typename K, typename V>
// typename bucket<K, V>iiket_t;
bool Hashtable<K, V>::resize(size_t new_bucket_count) {
  // Apply sizeof(T) * minsize shrink policy
  std::unique_ptr<bucket<K, V>[]> ptr(new bucket<K, V>[new_bucket_count]);
  bucket<K, V>* new_table = ptr.get();
  hash = hash_function<K>(new_bucket_count);  // new hash function from family

  // Rehash: add all entries of old table to new one
  for (int i = 0; i < m_bucket_count; ++i) {
    auto& bucket = m_buckets[i];
    for (auto const & entry : bucket) {
      size_t new_bucket_idx = hash(entry.first);
      if (new_bucket_idx < 0 || new_bucket_idx >= new_bucket_count) {
        // excpt. wrong hash index: public out_of_range
        return false;
      }
      auto& new_bucket = new_table[new_bucket_idx];
      if (!new_bucket.add(entry.first, entry.second)) {
        return false;
      }
    }
  }
  ptr.release();

  delete [] m_buckets;
  m_bucket_count = new_bucket_count;
  m_buckets = new_table;
  // m_size successfully added
  return true;
}

template <typename K, typename V>
bool Hashtable<K, V>::add(const K& key, const V& value) {
  size_t factor = load_factor();
  if ( factor > m_load_factor_bound_up ) {  // unlikely
    if (!resize(2*m_bucket_count))
      return false;
  } else if (factor <  m_load_factor_bound_low) {  // 0.4 // unlikely
    if (!resize(m_bucket_count/2))
      return false;
  }
  size_t bucket_idx  = hash(key);
  auto& bucket = m_buckets[bucket_idx];
  bool success = bucket.add(key, value);
  m_size += success ? 1 : 0;
  return success;
}

template <typename K, typename V>
bool Hashtable<K, V>::set(const K& key, const V& value) {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return false;
  }
  auto& bucket = m_buckets[bucket_idx];
  if (!bucket.set(key, value))
    return bucket.add(key, value);
  return true;
}

template <typename K, typename V>
const V* Hashtable<K, V>::get(const K& key) const {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return nullptr;
  }
  return m_buckets[bucket_idx].get(key);
}

template <typename K, typename V>
const V& Hashtable<K, V>::get(const K& key, bool *success) const {
  size_t bucket_idx = hash(key);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    _assert("bad bucket index",
            __ESR_PRETTY_FUNCTION__, __FILE__, __LINE__);
    // out_of_range exception
  }
  if (success != nullptr)
    *success = true;

  return *m_buckets[bucket_idx].get(key);
}

template <typename K, typename V>
typename Hashtable<K, V>::iterator Hashtable<K, V>::find(const K& key) {
  size_t bucket_idx = hash(key);
  auto& bucket = m_buckets[bucket_idx];
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return iterator(this, m_bucket_count-1, m_buckets[m_bucket_count-1].end());
    // return end();
  }
  auto end = bucket.end();
  for (auto it = bucket.begin(); it != end; ++it)
    if (key == it.key())
      return iterator(this, bucket_idx, it);

  return iterator(this, m_bucket_count-1, m_buckets[m_bucket_count-1].end());
  // return end();
}

template <typename K, typename V>
void Hashtable<K, V>::remove(const K& key) {
  size_t bucket_idx = hash(key, m_bucket_count);
  if (bucket_idx < 0 || bucket_idx >= m_bucket_count) {
    // out_of_range exception
    return;
  }
  bool success = m_buckets[bucket_idx].rm(key);
  m_size -= (success ? 1 : 0);
}

template <typename K, typename V>
ostream & operator<<(ostream & os, const Hashtable<K, V> & ht) {
  for (int i = 0; i < ht.m_bucket_count; ++i)
    os << std::setw(3) << i << ": " << ht.m_buckets[i];
  return os;
}

}  // namespace esr

#endif  // ESR_ESR_HASHTABLE_FLYMAKE_HPP_
