// Copyright 2016
#ifndef ESR_HASHER_FLYMAKE_HPP_
#define ESR_HASHER_FLYMAKE_HPP_

#include <cassert>  // assert().
#include <string>   // std::string.

namespace esr {

////////////////////////////////////////////////////////////////////////////////
/// @class hasher.
///
/// @brief Hash Function Interface.
/// Maps key to index in bucket array.
/// @tparam K type of hash key.
////////////////////////////////////////////////////////////////////////////////
template<typename K>
class hasher {
 public:
  /// @brief Default constructor, creates Hasher.
  /// @param cardinality is size a of bucket array.
  /// @return nothing.
  explicit hasher(size_t cardinality) : m_cardinality(cardinality) {}

  /// Returns hash code for a key.
  virtual uint64_t code(const K& key) const = 0;

  /// @brief Maps key to index of bucket array
  /// Obtains hash code using code() and maps it
  /// to index of bucket array.
  /// @param key is a hash key.
  /// @return Index of bucket array in a range
  /// from 1 to cardinality.
  size_t operator()(const K& key) const {
    assert(m_cardinality != 0);
    return code(key) % m_cardinality;
  }
 private:
  size_t m_cardinality;  ///< Size of bucket array.
};

////////////////////////////////////////////////////////////////////////////////
/// @class hashfunction.
///
/// @brief Hash Function Implementation.
/// Must be specialized for concrete type.
/// @tparam K type of hash key.
////////////////////////////////////////////////////////////////////////////////
template<typename K>
class hash_function : public hasher<K> {};

////////////////////////////////////////////////////////////////////////////////
// Hash functions for a few basic types int, char, bool and std::string.
////////////////////////////////////////////////////////////////////////////////

// integer hash function
#if 1  // in use
/// @brief Simple hash function for integer type.
/// Simple integer hash function, it has higher performance
/// then hash function from family of hashfunctions with primes.
/// @return hash code of integer key.
template <>
class hash_function<int> : public hasher<int> {
 public:
  explicit hash_function(int cardinality = 1) : hasher<int>(cardinality) {}
  uint64_t code(const int& key) const { return key; }
};
#else  // not used
/// @brief Simple hash function for integer type (not used).
/// Family of hashfunctions with primes. It has lower performance
/// then simple integer hash function.
/// @return hash code of integer key.
template <>
class hash_function<int> : public hasher<int> {
 public:
  uint32_t m_prime;
  uint32_t m_a;
  uint32_t m_b;
  explicit hash_function(int cardinality = 1, uint32_t prime = 10000019) :
      hasher<int>(cardinality),
      m_prime(prime) {
    std::srand(std::time(0));
    m_a = std::rand() %  (m_prime - 1) + 1;  // 1 <= a <= p - 1
    m_b = std::rand() %  (m_prime - 1);      // 1 <= b <= p - 1
  }
  uint64_t code(const int& key) const {
    return (m_a*key + m_b) % m_prime;
  }
};
#endif  // integer hash function

// boolean hash function
/// @brief Hash function for boolean type.
/// @return hash code of boolean key.
template <>
class hash_function<bool> : public hasher<bool> {
 public:
  explicit hash_function(size_t cardinality = 1) : hasher<bool>(cardinality) {}
  uint64_t code(const bool& key) const {
    return (key ? 1231 : 1237);
  }
};

// char hashfunction
/// @brief Hash function for char type.
/// @return hash code of char key.
/// @note Not tested for performance,
/// may be should be replaced with a simple one.
template <>
class hash_function<char> : public hasher<char> {
 public:
  uint32_t m_prime;
  uint32_t m_a;
  uint32_t m_b;
  explicit hash_function(int cardinality = 1, uint32_t prime = 389) :
      hasher<char>(cardinality),
      m_prime(prime) {
    std::srand(std::time(0));
    m_a = std::rand() %  (m_prime - 1) + 1;  // 1 <= a <= p - 1
    m_b = std::rand() %  (m_prime - 1);      // 1 <= b <= p - 1
  }
  uint64_t code(const char& key) const {
    return (m_a*key + m_b) % m_prime;
  }
};

// std::string hash function
#if 1  // in use
/// @brief "Java style" hash function for string type.
/// It is more effective then hash function with big prime.
/// @return hash code of integer key.
template <>
class hash_function<std::string> : public hasher<std::string> {
 public:
  uint32_t m_prime;
  uint32_t m_devider;
  explicit hash_function(size_t cardinality = 1,
                   size_t devider = 8,
                   size_t prime = 37) :
      hasher<std::string>(cardinality),
      m_devider(devider),
      m_prime(prime) {}

  uint64_t code(const std::string& key) const {
    uint64_t h = 0;
    int skip = 1 > (key.size() / m_devider) ? 1 : (key.size() / m_devider);
    for (int i = 0; i < key.size(); ++i)
      h = key.c_str()[i] + m_prime * h;
    return h;
  }
};
#else  // not used
/// @brief Hash function with big prime.
/// It is less effective then "Java style" hash function.
/// @return hash code of integer key.
template <>
class hash_function<std::string> : public hasher<std::string> {
 public:
  uint32_t m_prime;
  uint32_t m_multiplier;
  explicit hash_function(size_t cardinality = 1,
                   size_t multiplier = 263,
                   size_t prime = 1000000007) :
      hasher<std::string>(cardinality),
      m_multiplier(multiplier),
      m_prime(prime) {}

  uint64_t code(const std::string& key) const {
    uint64_t h = 0;
    for (int i = static_cast<int>(key.size()) - 1; i >= 0; --i)
      h = (h * m_multiplier + key[i]) % m_prime;
    return h;
  }
};
#endif  // std::string hash function

}  // namespace esr

#endif  // ESR_HASHER_FLYMAKE_HPP_
