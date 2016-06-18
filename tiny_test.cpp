// Copyright 2016
#include <iostream>
#include <string>
#include <stdexcept>

#include <esr/hashtable.hpp>
#include <esr/hashexcept.hpp>

int main(int argc, char *argv[]) {
  esr::Hashtable<int, int> table;
  // esr::Hashtable<int, int>::iterator ee = table.end();
  // *ee;


  esr::Hashtable<std::string, std::string> phone_book;
  phone_book.add("Fima", "01034900103");
  phone_book.add("Chicha", "01070300103");
  phone_book.add("Kit", "01070300777");

  auto record = phone_book.find("Fima");
  if (record != phone_book.end())
    std::cout << record->key() << ", " << record->value()  << '\n';

  for (auto& record : phone_book)
    std::cout << record.key() << ", " << record.value()  << '\n';


  
  try {
    table.add(5, 8);
    table.add(7, 8);
    std::cerr << table << '\n';
    // esr::Hashtable<int, int>::iterator ee = table.end();
    // *ee;
  } catch (esr::exception::end_iterator& e) {
    std::cerr << "error iterating hashtable: "
              << e.what() << '\n'
              << " current bucket index: "
              << e.current_bucket_index() << '\n';
    throw;
  } catch (esr::exception::bucket_index& e) {
    std::cerr << "error adding to hashtable: "
              << e.what() << '\n'
              << " index: " << e.index() << '\n';
    throw;
  } catch (esr::exception::hashtable& e) {
    std::cerr << "add to hashtable exception: " << e.what() << '\n';
    std::cerr << " thrown by: " << e.who() << '\n';
    throw;
  } catch(...) {
    std::cerr << "unexpecetd error\n";
    throw;
  }

  esr::Hashtable<int, int> table_copy(table);
  std::cerr << "Copy\n";
  table_copy.add(8, 8);
  table.add(8, 8);
  std::cerr << table_copy << '\n';

  std::cerr << "Table\n";
  std::cerr << table << '\n';

  esr::Hashtable<int, int> table_assigned;
  table_assigned = table;
  std::cerr << "Assignment\n";

  table_assigned.add(15, 8);   // hash() the same
  table.add(15, 8);
  std::cerr << table_assigned << '\n';

  std::cerr << "Table\n";
  std::cerr << table << '\n';

  std::cerr << "Iterator\n";
  esr::Hashtable<int, int>::iterator begin = table.begin();
  esr::Hashtable<int, int>::iterator end = table.end();
  esr::Hashtable<int, int>::iterator end_second = table.end();

  if (end == end_second)
    std::cerr << "[success]\n";

  if (begin != end)
    std::cerr << "[success]\n";

  esr::Hashtable<int, int>::iterator it = table.begin();
  std::cerr << it->key() << ',' << it->value() << '\n';
  ++it;
  std::cerr << (*it).key() << ',' << (*it).value() << '\n';

  std::cerr << "Iterate\n";
  for (auto& i : table) {
    std::cerr << i.key() << ',' << i.value() << '\n';
    // i.second = 5;
  }

  std::cerr << "Modified\n";
  for (auto& i : table)
    std::cerr << i.key() << ',' << i.value() << '\n';

  esr::Hashtable<int, int>::iterator found = table.find(15);
  esr::Hashtable<int, int>::iterator e = table.end();
  if (found != e)
    std::cerr << found->key() << ',' << found->value() << '\n';

  std::cerr << "Get\n";
  const int* value = table.get(15);
  if (value != nullptr)
    std::cerr << *value << '\n';

  table.set(15, 888);
  value = table.get(15);
  if (value != nullptr)
    std::cerr << *value << '\n';

  std::cerr << "Deletion\n";
  std::cerr << table << '\n';
  table.remove(15);
  std::cerr << table << '\n';

  std::cerr << "Insertion\n";
  esr::Hashtable<int, int> table_resized;
  for (int i = 0; i < 32; ++i)
    table_resized.add(i, i);

  std::cerr << table_resized << '\n';

  std::cerr << "Deletion\n";
  for (int i = 0; i < 32; ++i)
    table_resized.remove(i);

  std::cerr << table_resized << '\n';

  std::cerr << "Duplicate keys\n";
  esr::Hashtable<std::string, int> table_dup_strings;
  esr::Hashtable<int, int> table_dup_ints;
  for (int i = 0; i < 16; ++i) {
    table_dup_strings.add("duplicate", i);
    table_dup_ints.add(5, i);
  }
  std::cerr << table_dup_strings << '\n';
  std::cerr << table_dup_ints << '\n';

  return 0;
}

