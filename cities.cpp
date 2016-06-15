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
    // std::cout << __ESR_PRETTY_FUNCTION__ << '\n';
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

  std::vector<city::city> STL_CITIES;

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
      STL_CITIES.push_back(city);
    }
  }
  file.close();

  esr::Hashtable<city::hkey, uint32_t> city_data_table;

  std::cout << "DATA: " << std::flush;
  for (auto& city : STL_CITIES) {
    city::hkey key(city);
    city_data_table.add(key, city.population);
  }
  std::cout << city_data_table.size() << " entries\n";

  std::cout << "YEARS: " << std::flush;
  esr::Hashtable<int, uint16_t> city_year_table;
  auto end = city_year_table.end();
  for (auto& city : city_data_table) {
    auto found = city_year_table.find(city.key().year);
    if (found != end)
      ++found->value();
    else
      city_year_table.add(city.key().year, 1);
  }
  std::cout << city_year_table.size() << " entries \n";
  for (auto& year : city_year_table)
    std::cout << " " << year.value()  << " cities in "
              << " " << year.key() << "\n";

  std::cout << "STATUS: " << std::flush;
  esr::Hashtable<std::string, bool> city_status_table;
  for (auto& city : city_data_table)
    city_status_table.add(city.key().name, city.key().is_capital);
  std::cout << city_status_table.size() << " entries.\n";

  uint32_t number_of_capitals = 0;
  uint32_t number_of_secondaries = 0;
  for (auto& status : city_status_table) {
    bool is_capital = status.value();
    if (is_capital)
      ++number_of_capitals;
    else
      ++number_of_secondaries;
  }
  std::cout << " " << number_of_capitals << " capitals\n"
            << " " << number_of_secondaries << " secondaries\n";

  std::cout << "AREA: " << std::flush;
  esr::Hashtable<std::string, int> city_area_table;
  for (auto& city : city_data_table)
    city_area_table.add(city.key().name, city.key().area);
  std::cout << city_area_table.size() << " entries\n";


  uint64_t total_area = 0;
  for (auto& city : city_area_table)
      total_area += city.value();

  std::cout << " " << total_area << " sq.km of "
            << city_area_table.size() << " cities.\n";

  std::cout << "STATE: " << std::flush;
  esr::Hashtable<std::string, std::string> city_state_table;
  esr::Hashtable<std::string, int> city_state_counter_table;
  for (auto& city : city_data_table) {
    city_state_table.add(city.key().name, city.key().state);
    auto found = city_state_counter_table.find(city.key().state);
    if (found != city_state_counter_table.end())
      ++found->value();
    else
      city_state_counter_table.add(city.key().state, 1);
  }

  std::cout << "total " << city_state_counter_table.size() << "\n";
  std::cout << city_state_counter_table <<'\n';


  

  
  /*
  std::cout << "Iterator test: " << std::flush;
  for (auto& city : city_data_table) {
    std::cout << city.key().year << " - "
              << city.key().name << ": "
              << city.value() << '\n' << std::flush;
  }
  */
  /*
  std::cout << "Cities" << std::flush;
  for (auto& city : STL_CITIES) {
    city::hkey key(city);
    city_data_table.add(key, city.population);
  }
  */ 
  
  return ret::success;
}

