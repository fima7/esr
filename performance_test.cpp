// Copyright 2016
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <esr/hashtable.hpp>

////////////////////////////////////////////////////////////////////////////////
// Integer Keys Iserions and Retrievals
////////////////////////////////////////////////////////////////////////////////
void insertion_to_Hashtable(esr::Hashtable<int, int>* table,
                            size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i)
    table->add(i, i);
}

void insertion_to_unordered_map(std::unordered_map<int, int>* map,
                                size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i)
    map->insert(std::make_pair(i, i));
}

void retrieval_from_Hashtable(esr::Hashtable<int, int>* table,
                              size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i) {
    auto found = table->find(i);
    if (found == table->end())
      std::cerr << "fail\n";
  }
}

void retrieval_from_unordered_map(std::unordered_map<int, int>* map,
                                  size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i) {
    auto found = map->find(i);
    if (found == map->end())
      std::cerr << "fail\n";
  }
}

////////////////////////////////////////////////////////////////////////////////
// String Keys Iserions and Retrievals
////////////////////////////////////////////////////////////////////////////////
void insertion_to_Hashtable(esr::Hashtable<std::string, int>* table,
                            const std::vector<std::string>& keys,
                            size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i)
    table->add(keys[i], i);
}

void insertion_to_unordered_map(std::unordered_map<std::string, int>* map,
                            const std::vector<std::string>& keys,
                            size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i)
    map->insert(std::make_pair(keys[i], i));
}

void retrieval_from_Hashtable(esr::Hashtable<std::string, int>* table,
                              const std::vector<std::string>& keys,
                              size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i) {
    auto found = table->find(keys[i]);
    if (found == table->end())
      std::cerr << "fail\n";
  }
}

void retrieval_from_unordered_map(std::unordered_map<std::string, int>* map,
                                  const std::vector<std::string>& keys,
                                  size_t number_of_entries) {
  for (int i = 0; i < number_of_entries; ++i) {
    auto found = map->find(keys[i]);
    if (found == map->end())
      std::cerr << "fail\n";
  }
}

////////////////////////////////////////////////////////////////////////////////
// Stop Watch
////////////////////////////////////////////////////////////////////////////////
namespace esr {

class stopwatch {
 public:
  void start() { m_start = clock(); }
  void stop() { m_stop = clock(); }
  double time() {  // milliseconds
    return (m_stop - m_start)/static_cast<double>(CLOCKS_PER_SEC)*1000;
  }
 private:
  clock_t m_start;
  clock_t m_stop;
};

}  // namespace esr

int main(int argc, char *argv[]) {
  esr::stopwatch stopwatch;

  int n = 2;

  std::cout << "Integer Keys\n";
  for (int i = 0; i < 20; ++i, n += n) {
    esr::Hashtable<int, int>* table = new  esr::Hashtable<int, int>();
    std::unordered_map<int, int>* map = new std::unordered_map<int, int>();

    std::cout << n << ' ';

    // Insertion
    stopwatch.start();
    insertion_to_Hashtable(table, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed <<stopwatch.time()/n << ' ';

    stopwatch.start();
    insertion_to_unordered_map(map, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed << stopwatch.time()/n << ' ';

    // Retrieval
    stopwatch.start();
    retrieval_from_Hashtable(table, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    stopwatch.start();
    retrieval_from_unordered_map(map, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    delete map;
    delete table;

    std::cout << '\n' << std::flush;
  }

  std::cout << "Fixed Length String Keys\n";

  std::ifstream file("./data/unique_strings.txt");
  if (!file.is_open()) {
    std::cerr << "test error: couldn't open file \"unique_strings.txt\"\n";
    return false;
  }
  std::vector<std::string> keys;
  while (file.good()) {
    std::string key;
    file >> key;
    keys.push_back(key);
  }
  file.close();
  n = 2;
  for (int i = 0; i < 25 ; ++i, n += n) {
    if (n > keys.size())
      break;

    auto* table = new  esr::Hashtable<std::string, int>();
    auto* map = new std::unordered_map<std::string, int>();

    std::cout << n << ' ';

    // Insertion
    stopwatch.start();
    insertion_to_Hashtable(table, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed << stopwatch.time()/n << ' ';

    stopwatch.start();
    insertion_to_unordered_map(map, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed << stopwatch.time()/n << ' ';

    // Retrieval
    stopwatch.start();
    retrieval_from_Hashtable(table, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    stopwatch.start();
    retrieval_from_unordered_map(map, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    delete map;
    delete table;

    std::cout << '\n' << std::flush;
  }

  std::cout << "Variable Length String Keys\n";

  n = 2;
  for (int i = 0; i < 10; ++i) {
    n = keys.size();

    auto* table = new  esr::Hashtable<std::string, int>();
    auto* map = new std::unordered_map<std::string, int>();

    size_t key_size = keys[0].size();
    std::cout << key_size << ' ';

    // Insertion
    stopwatch.start();
    insertion_to_Hashtable(table, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed << stopwatch.time()/n << ' ';

    stopwatch.start();
    insertion_to_unordered_map(map, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << std::fixed << stopwatch.time()/n << ' ';

    // Retrieval
    stopwatch.start();
    retrieval_from_Hashtable(table, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    stopwatch.start();
    retrieval_from_unordered_map(map, keys, n);
    stopwatch.stop();
    std::cout << std::setw(8) << stopwatch.time()/n << ' ';

    delete map;
    delete table;

    std::cout << '\n' << std::flush;

    for (auto& key : keys)
      key += key;
  }

  return 0;
}

