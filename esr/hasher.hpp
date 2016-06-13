// Copyright 2016
#ifndef ESR_ESR_HASHER_FLYMAKE_HPP_
#define ESR_ESR_HASHER_FLYMAKE_HPP_

#include <string>

namespace esr {

template<typename K>
class hasher {
 public:
  explicit hasher(size_t cardinality) : m_cardinality(cardinality) {}
  virtual uint64_t code(const K& key) const = 0;
  size_t operator()(const K& key) const {
    return code(key) % m_cardinality;
  }
 private:
  size_t m_cardinality;
};

template<typename K>
class hash_function : public hasher<K> {};

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

template <>
class hash_function<bool> : public hasher<bool> {
 public:
  explicit hash_function(size_t cardinality = 1) : hasher<bool>(cardinality) {}
  uint64_t code(const bool& key) const {
    return (key ? 1231 : 1237);
  }
};

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

#if 1
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
#elif
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
    int skip = 1 > (key.size() / 8) ? 1 : (key.size() / 8);
    for (int i = 0; i < key.size(); ++i)
      h = key.c_str()[i] + 37 * h;
    return h;
  }
};
#endif

}  // namespace esr

#endif  // ESR_ESR_HASHER_FLYMAKE_HPP_
