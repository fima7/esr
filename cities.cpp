// Copyright 2016
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ostream>

#include <esr/hashtable.hpp>

const size_t kWordsInLineOfDataFile = 6;

namespace ret {
enum {
  success = 0,
  invalid_args = 1,
  file_error = 2
};
}

namespace city {
struct city {
  std::string name;
  bool is_capital;
  uint16_t year;
  uint32_t population;
  uint32_t area;
  std::string state;

  friend std::ostream & operator<<(std::ostream & os, const city & ct) {
    os << ct.name << " of " << ct.state << "(" << ct.year << ") : "
       << (ct.is_capital ? "capital" : "secondary") << ", "
       << "population is " << ct.population << " "
       << "on " << ct.area << " sq.km";
    return os;
  }
};

struct hkey {
  std::string name;
  bool is_capital;
  uint16_t year;
  uint32_t area;
  std::string state;
  hkey() {}
  explicit hkey(const city & ct) :
      name(ct.name),
      is_capital(ct.is_capital),
      year(ct.year),
      area(ct.area),
      state(ct.state)
  {}
  bool operator==(const hkey &other) const {
    if (name == other.name &&
        is_capital == other.is_capital &&
        year == other.year &&
        area == other.area &&
        state == other.state)
      return true;
    return false;
  }

  friend std::ostream & operator<<(std::ostream & os, const hkey & key) {
    os << '{'
       << key.name << ", "
       << key.is_capital << ", "
       << key.year << ", "
       << key.area << ", "
       << key.state
       << "}";
    return os;
  }
};

}  // namespace city


namespace esr {

template <>
class hash_function<city::hkey> :
      public esr::hasher<city::hkey> {
 public:
  explicit hash_function(size_t cardinality,
                         size_t start = 17,
                         size_t prime = 31) :
      esr::hasher<city::hkey>(cardinality),
      m_bool_hasher(cardinality),
      m_int_hasher(cardinality),
      m_string_hasher(cardinality),
      m_start(start),
      m_prime(prime) {
    std::cout << __ESR_PRETTY_FUNCTION__ << '\n';
  }

  uint64_t code(const city::hkey& key) const {
    uint64_t h = m_start;
    h = m_prime * h + m_bool_hasher.code(key.is_capital);
    h = m_prime * h + m_string_hasher.code(key.name);
    h = m_prime * h + m_int_hasher.code(key.year);
    h = m_prime * h + m_int_hasher.code(key.area);
    h = m_prime * h + m_string_hasher.code(key.state);
    return h;
  }
 private:
  size_t m_start;
  size_t m_prime;
  esr::hash_function<bool> m_bool_hasher;
  esr::hash_function<int> m_int_hasher;
  esr::hash_function<std::string> m_string_hasher;
};

}  // namespace esr

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage " << argv[0] << " file\n";
    return -ret::invalid_args;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "error: Couldn't open file \"" << argv[1] << "\"\n";
    return -ret::file_error;
  }

  std::vector<city::city> cities;

  while (file.good()) {
    std::vector<std::string> line;
    for (int i = 0; file.good() && i < kWordsInLineOfDataFile; ++i) {
      std::string word;
      std::getline(file, word, ';');
      word.erase(std::remove_if(word.begin(),
                                word.end(),
                                [](char c) {
                                  return (c =='\t' ||
                                          c == ' ' ||
                                          c == '\n' ||
                                          c == '\r');
                                }),
                 word.end());
      line.push_back(word);
    }
    if (line.size() == kWordsInLineOfDataFile) {
      city::city city;
      city.name = line[0];
      city.is_capital = std::stoul(line[1]);
      city.year = std::stoul(line[2]);
      city.population = std::stoul(line[3]);
      city.area = std::stoul(line[4]);
      city.state = line[5];
      cities.push_back(city);
    }
  }
  file.close();

  esr::Hashtable<city::hkey, uint32_t> city_hash;
  for (auto city : cities) {
    city::hkey key(city);
    city_hash.add(key, city.population);
  }

  for (auto city : cities) {
    city::hkey key(city);
    // city_hash.add(key, city.population);
  }
  std::cout << city_hash << '\n';
  return ret::success;
}

