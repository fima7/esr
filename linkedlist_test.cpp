// Copyright 2016
#include <iostream>
#include <esr/linkedlist.hpp>
////////////////////////////////////////////////////////////////////////////////
// Linked List Tests
////////////////////////////////////////////////////////////////////////////////
struct fake {
  int m_data;
  virtual ~fake() = 0;
  bool operator==(const fake& rhs) const {
    return m_data == rhs.m_data;
  }
  bool operator!=(const fake& rhs) const {
    return m_data != rhs.m_data;
  }
  friend ostream & operator<<(ostream & os, const fake& fk) {
    return os << fk.m_data;
  }
};
fake::~fake() {}

struct fake_key : public fake {};
struct fake_value: public fake {};

bool linkedlist_access_test(size_t size, bool verbose) {
  std::cout << "Access test: " << std::flush;

  esr::linkedlist<int, int> ll;
  int key;
  int value;

  // Add
  for (key = 0, value = size - 1; key < size; ++key, --value)
    ll.push_front(key, value);

  if (verbose) {
    std::cout << "\nlist<int, int> = { " << ll << "}\n" << std::flush;
  }

  // Find and check
  for (key = size - 1, value = 0; key >=0;  --key, ++value) {
    const esr::listnode<int, int>* found = ll.find(key);
    if (found == nullptr) {
      std::cout << "<int,int> no value found for key = " << key
                << '\n' << std::flush;
      return false;
    }

    if (found->key() != key || found->value() != value) {
      std::cout << "<int,int> found "
                << "key = " << found->key() << " , "
                << "value = " << found->value() << " "
                << "doesn't match expected "
                << "key = " << key << ", "
                << "value = " << value << ". "
                << std::flush;
      return false;
    }
  }

  esr::linkedlist<fake_key, fake_value> fake_list;
  fake_key* expect_keys = new fake_key[size];
  fake_value* expect_values = new fake_value[size];

  // Add
  key = 0, value = size - 1;
  for (int i = 0; i < size; ++i) {
    expect_keys[i].m_data = key;
    expect_values[i].m_data = value;

    fake_key fkey;
    fake_value fvalue;
    fkey.m_data = key;
    fvalue.m_data = value;
    fake_list.push_front(fkey, fvalue);

    ++key, --value;
  }

  if (verbose) {
    std::cout << "\nlist<fake_key, fake_value> = { "
              << fake_list << "}\n" << std::flush;
  }

  // Find and check
  for (int i = size - 1; i >= 0; --i) {
    const esr::listnode<fake_key, fake_value>* found;
    found = fake_list.find(expect_keys[i]);

    if (found == nullptr) {
      std::cout << "<fake_key,fake_value> no value found for key = "
                << expect_keys[i]
                << '\n' << std::flush;
      return false;
    }

    const fake_key& fkey = found->key();
    const fake_value& fvalue = found->value();

    if (fkey != expect_keys[i] || fvalue != expect_values[i]) {
      std::cout << "<fake_key,fake_value> found "
                << "key = " << found->key() << " , "
                << "value = " << found->value() << " "
                << "doesn't match expected "
                << "key = " << key << ", "
                << "value = " << value << ". "
                << std::flush;
      return false;
    }
  }

  // Find and modify
  for (int i = size - 1; i >= 0; --i) {
    esr::listnode<fake_key, fake_value>* found;
    found = fake_list.find(expect_keys[i]);

    if (found == nullptr) {
      std::cout << "<fake_key,fake_value> no value found for key = "
                << expect_keys[i]
                << '\n' << std::flush;
      return false;
    }

    const fake_key& fkey = found->key();
    fake_value& fvalue = found->value();
    expect_values[i].m_data -= size - 1;
    fvalue.m_data -= size - 1;
  }

  if (verbose) {
    std::cout << "\nmodified list<fake_key, fake_value> = { "
              << fake_list << "}\n" << std::flush;
  }

  // Find and check
  for (int i = size - 1; i >= 0; --i) {
    esr::listnode<fake_key, fake_value>* found;
    found = fake_list.find(expect_keys[i]);

    if (found == nullptr) {
      std::cout << "<fake_key,fake_value> no value found for key = "
                << expect_keys[i]
                << '\n' << std::flush;
      return false;
    }

    const fake_key& fkey = found->key();
    fake_value& fvalue = found->value();

    if (fkey != expect_keys[i] || fvalue != expect_values[i]) {
      std::cout << "<fake_key,fake_value> found "
                << "key = " << fkey.m_data << " , "
                << "value = " << fvalue.m_data << " "
                << "doesn't match expected "
                << "key = " << key << ", "
                << "value = " << value << ". "
                << std::flush;
      return false;
    }
  }

  delete [] expect_keys;
  delete [] expect_values;

  return true;
}

bool linkedlist_copy_test(size_t size, bool verbose) {
  std::cout << "Copy test: " << std::flush;

  int key;
  int value;
  esr::linkedlist<int, int> ll_src;

  // Add
  for (key = 0, value = size - 1; key < size; ++key, --value)
    ll_src.push_front(key, value);

  if (verbose) {
    std::cout << "\nsource list<int, int> = { "
              << ll_src << "}\n" << std::flush;
  }

  esr::linkedlist<int, int> ll_dst(ll_src);

  if (verbose) {
    std::cout << "\ndestination list<int, int> = { "
              << ll_dst << "}\n" << std::flush;
  }

  // Check
  for (key = 0, value = size - 1; key < size; ++key, --value) {
    const esr::listnode<int, int>* found_src = ll_src.find(key);
    const esr::listnode<int, int>* found_dst = ll_dst.find(key);

    if (found_src == nullptr) {
      std::cout << "<int,int> source has no value found for key = "
                << key << '\n' << std::flush;
      return false;
    }

    if (found_dst == nullptr) {
      std::cout << "<int,int> destination has no value found for key = "
                << key << '\n' << std::flush;
      return false;
    }

    // Check source
    if (found_src->key() != key || found_src->value() != value) {
      std::cout << "<int,int> source "
                << "key = " << found_src->key() << " , "
                << "value = " << found_src->value() << " "
                << "doesn't match expected "
                << "key = " << key << ", "
                << "value = " << value << ". "
                << std::flush;
      return false;
    }
    // Check destination
    if (found_dst->key() != found_src->key() ||
        found_dst->value() != found_src->value()) {
      std::cout << "<int,int> destination "
                << "key = " << found_dst->key() << " , "
                << "value = " << found_dst->value() << " "
                << "doesn't match source "
                << "key = " << found_src->key() << ", "
                << "value = " << found_src->key() << ". "
                << std::flush;
      return false;
    }
  }
  return true;
}

bool linkedlist_assignment_test(size_t size, bool verbose) {
  std::cout << "Assignment test: " << std::flush;

  int key;
  int value;
  esr::linkedlist<int, int> ll_src;

  // Add
  for (key = 0, value = size - 1; key < size; ++key, --value)
    ll_src.push_front(key, value);

  if (verbose) {
    std::cout << "\nsource list<int, int> = { "
              << ll_src << "}\n" << std::flush;
  }

  esr::linkedlist<int, int> ll_dst;
  ll_dst = ll_src;

  if (verbose) {
    std::cout << "\ndestination list<int, int> = { "
              << ll_dst << "}\n" << std::flush;
  }

  // Check
  for (key = 0, value = size - 1; key < size; ++key, --value) {
    const esr::listnode<int, int>* found_src = ll_src.find(key);
    const esr::listnode<int, int>* found_dst = ll_dst.find(key);

    if (found_src == nullptr) {
      std::cout << "<int,int> source has no value found for key = "
                << key << '\n' << std::flush;
      return false;
    }

    if (found_dst == nullptr) {
      std::cout << "<int,int> destination has no value found for key = "
                << key << '\n' << std::flush;
      return false;
    }

    // Check source
    if (found_src->key() != key || found_src->value() != value) {
      std::cout << "<int,int> source "
                << "key = " << found_src->key() << " , "
                << "value = " << found_src->value() << " "
                << "doesn't match expected "
                << "key = " << key << ", "
                << "value = " << value << ". "
                << std::flush;
      return false;
    }
    // Check destination
    if (found_dst->key() != found_src->key() ||
        found_dst->value() != found_src->value()) {
      std::cout << "<int,int> destination "
                << "key = " << found_dst->key() << " , "
                << "value = " << found_dst->value() << " "
                << "doesn't match source "
                << "key = " << found_src->key() << ", "
                << "value = " << found_src->key() << ". "
                << std::flush;
      return false;
    }
  }
  return true;
}


bool linkedlist_deletion_test(size_t size, bool verbose) {
  std::cout << "Deletion test: " << std::flush;

  esr::linkedlist<int, int> ll_expect;
  esr::linkedlist<int, int> ll_tmp;
  int key;
  int value;

  // Add
  for (key = 0, value = size - 1; key < size; ++key, --value)
    ll_expect.push_front(key, value);

  if (verbose) {
    std::cout << "\nlist<int, int> = { " << ll_expect << "}\n" << std::flush;
  }

  // Delete from front
  ll_tmp = ll_expect;
  for (int key = 0; key < size; ++key) {
    ll_tmp.erase(key);
    const esr::listnode<int, int>* found = ll_tmp.find(key);
    if (found != nullptr) {
      std::cout << "<int,int> found deleted value for key = "
                << key << '\n' << std::flush;
      return false;
    }
    for (int key_rest = key + 1; key_rest < size; ++key_rest) {
      const esr::listnode<int, int>* found_rest = ll_tmp.find(key_rest);
      const esr::listnode<int, int>* found_expect = ll_expect.find(key_rest);
      if (found_rest == nullptr) {
        std::cout << "<int,int> no element by with key = "
                  << key_rest << '\n' << std::flush;
        return false;
      }
      // Compare with expectations
      if (found_rest->key() != found_expect->key()
          || found_rest->value() != found_expect->value()) {
        std::cout << "<int,int> found"
                  << "key = " << found_rest->key() << " , "
                  << "value = " << found_rest->value() << " "
                  << "doesn't match expected "
                  << "key = " << found_expect->key() << ", "
                  << "value = " << found_expect->value() << ". "
                  << std::flush;
        return false;
      }
    }
  }

  // Delete from back
  ll_tmp = ll_expect;
  for (int key = size - 1; key >= 0; --key) {
    ll_tmp.erase(key);
    const esr::listnode<int, int>* found = ll_tmp.find(key);
    if (found != nullptr) {
      std::cout << "<int,int> found deleted value for key = "
                << key << '\n' << std::flush;
      return false;
    }
    for (int key_rest = key - 1; key_rest >= 0; --key_rest) {
      const esr::listnode<int, int>* found_rest = ll_tmp.find(key_rest);
      const esr::listnode<int, int>* found_expect = ll_expect.find(key_rest);
      if (found_rest == nullptr) {
        std::cout << "<int,int> no element by with key = "
                  << key_rest << '\n' << std::flush;
        return false;
      }
      // Compare with expectations
      if (found_rest->key() != found_expect->key()
          || found_rest->value() != found_expect->value()) {
        std::cout << "<int,int> found"
                  << "key = " << found_rest->key() << " , "
                  << "value = " << found_rest->value() << " "
                  << "doesn't match expected "
                  << "key = " << found_expect->key() << ", "
                  << "value = " << found_expect->value() << ". "
                  << std::flush;
        return false;
      }
    }
  }

  // Delete previous to back
  ll_tmp = ll_expect;
  ll_tmp.erase(size - 2);

  const esr::listnode<int, int>* found = ll_tmp.find(size - 2);
  if (found != nullptr) {
    std::cout << "<int,int> found deleted value for key = "
              << key << '\n' << std::flush;
    return false;
  }

  for (int key_rest = 0; key_rest < size; ++key_rest) {
    if (key_rest == size - 2)
      continue;
    const esr::listnode<int, int>* found_rest = ll_tmp.find(key_rest);
    const esr::listnode<int, int>* found_expect = ll_expect.find(key_rest);
    if (found_rest == nullptr) {
      std::cout << "<int,int> no element by with key = "
                << key_rest << '\n' << std::flush;
      return false;
    }
    // Compare with expectations
    if (found_rest->key() != found_expect->key()
        || found_rest->value() != found_expect->value()) {
      std::cout << "<int,int> found"
                << "key = " << found_rest->key() << " , "
                << "value = " << found_rest->value() << " "
                << "doesn't match expected "
                << "key = " << found_expect->key() << ", "
                << "value = " << found_expect->value() << ". "
                << std::flush;
      return false;
    }
  }

  // Next to front
  ll_tmp = ll_expect;
  ll_tmp.erase(1);
  found = nullptr;
  found = ll_tmp.find(1);
  if (found != nullptr) {
    std::cout << "<int,int> found deleted value for key = "
              << key << '\n' << std::flush;
    return false;
  }

  for (int key_rest = 0; key_rest < size; ++key_rest) {
    if (key_rest == 1)
      continue;
    const esr::listnode<int, int>* found_rest = ll_tmp.find(key_rest);
    const esr::listnode<int, int>* found_expect = ll_expect.find(key_rest);
    if (found_rest == nullptr) {
      std::cout << "<int,int> no element by with key = "
                << key_rest << '\n' << std::flush;
      return false;
    }
    // Compare with expectations
    if (found_rest->key() != found_expect->key()
        || found_rest->value() != found_expect->value()) {
      std::cout << "<int,int> found"
                << "key = " << found_rest->key() << " , "
                << "value = " << found_rest->value() << " "
                << "doesn't match expected "
                << "key = " << found_expect->key() << ", "
                << "value = " << found_expect->value() << ". "
                << std::flush;
      return false;
    }
  }
  return true;
}

bool fake_object_test() {
  std::cout << "Fake object test: " << std::flush;

  fake_key fkey[2];
  fake_value fvalue[2];
  
  fkey[0].m_data = 1;
  fkey[1].m_data = 1;

  if (fkey[0] == fkey[1])
    std::cout << fkey[0] << ',' << fkey[1] << ' '
              << "are same"<< std::flush;
  else
    std::cout << fkey[0] << ',' << fkey[1] << ' '
              << "are different" << std::flush;

  std::cout << "; " << std::flush;

  fkey[0].m_data = 1;
  fkey[1].m_data = 2;
  if (fkey[0] != fkey[1])
    std::cout << fkey[0] << ',' << fkey[1] << ' '
              << "are different" << std::flush;
  else
    std::cout << fkey[0] << ',' << fkey[1] << ' '
              << "are same" << std::flush;

  std::cout << ". " << std::flush;

  return true;
}

const int kLinkeListSize = 1024;

int main(int argc, const char * argv[]) {
  if (!fake_object_test())
    std::cout << "[FAILED]\n" << std::flush;
  else
    std::cout << "[PASSED]\n" << std::flush;

  if (!linkedlist_access_test(kLinkeListSize, false))
    std::cout << "[FAILED]\n" << std::flush;
  else
    std::cout << "[PASSED]\n" << std::flush;
  if (!linkedlist_copy_test(kLinkeListSize, false))
    std::cout << "[FAILED]\n" << std::flush;
  else
    std::cout << "[PASSED]\n" << std::flush;
  if (!linkedlist_assignment_test(kLinkeListSize, false))
    std::cout << "[FAILED]\n" << std::flush;
  else
    std::cout << "[PASSED]\n" << std::flush;
  if (!linkedlist_deletion_test(kLinkeListSize, false))
    std::cout << "[FAILED]\n" << std::flush;
  else
    std::cout << "[PASSED]\n" << std::flush;
  return 0;
}
