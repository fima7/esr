// Copyright 2016
#ifndef ESR_HASHTEST_FLYMAKE_HPP_
#define ESR_HASHTEST_FLYMAKE_HPP_

#include <string>

#include <esr/hashtable.hpp>
#include <esr/assert.hpp>

namespace esr_test {

class CorrectnessTest {
 public:
  explicit CorrectnessTest(int input_size = 1024,
                           const std::string & description = "",
                           const std::string & name = "unknown") :
      m_input_size(input_size),
      m_description(description),
      m_name(name) {}
  const std::string& name() const { return m_name; }
  const int intput_size() const { return m_input_size; }
  const std::string& description() const { return m_description; }
  virtual bool make_expected_hashtable() = 0;
  virtual bool run() = 0;
  virtual ~CorrectnessTest() { /* std::cout << "CorrectnessTest dtor\n"; */ }

 private:
  int m_input_size;
  std::string m_name;
  std::string m_description;
};

template <typename K, typename V>
class InsertionRetrievalTest : public CorrectnessTest {
 public:
  typedef typename esr::Hashtable<K, V>::iterator hashtable_iterator_t;

  explicit InsertionRetrievalTest(int intput_size = 1024,
                                  const std::string &
                                  description = "",
                                  const std::string &
                                  name = "InsertionRetrievalTest") :
      CorrectnessTest(intput_size, description, name) , m_test_table() {}
  virtual ~InsertionRetrievalTest() {}
  virtual bool make_expected_hashtable();

  virtual bool run() {
    // std::cout << __ESR_PRETTY_FUNCTION__ << ": ";
    if (m_positive_table.empty()) {
      std::cout << "expected hashtable empty in "
                << __ESR_PRETTY_FUNCTION__ << '\n'
                << std::flush;
      return false;
    }

    // bool add(const K& key, const V& value);
    if (!add_positive_negative()) {
      std::cout << "Unexpected insertion behavour. " << std::flush;
      return false;
    }

    // const V* get(const K& key) const;
    // const V& get(const K& key, bool *success) const;
    // iterator find(const K& key);
    if (!get_by_pointer_positive()   || !get_by_pointer_negative() ||
        !find_positive() || !find_negative()) {
      std::cout << "Unexpected retrieval behavour. " << std::flush;
       return false;
    }
    // std::cout << m_test_table << '\n';
    return true;
  }

 protected:
  esr::Hashtable<K, V> m_test_table;
  std::unordered_map<K, V> m_positive_table;
  std::unordered_map<K, V> m_negative_table;

  virtual bool add_positive() {
    for (auto & key_value : m_positive_table) {
      const K& key = key_value.first;
      const V& value = key_value.second;
      bool success = m_test_table.add(key, value);
      if (!success) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "failed to add(key = " << key << ", "
                  << "value = " << value << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }

 private:
  virtual bool add_positive_negative() {
    if (!add_positive())
      return false;

    // attempt two insert twice
    for (auto & key_value : m_positive_table) {
      const K& key = key_value.first;
      const V& value = key_value.second;
      bool success = m_test_table.add(key, value);
      if (success) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "added twice (key = " << key << ", "
                  << "value = " << value << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }

  virtual bool get_by_pointer_positive() {
    // const V* get(const K& key) const;
    for (auto & key_value : m_positive_table) {
      const K& e_key = key_value.first;
      const V& e_value = key_value.second;
      const V* r_value  = m_test_table.get(e_key);
      if (r_value == nullptr) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "no value for for key = " << e_key << ", "
                  << "expected value = " << e_value << ". "
                  << std::flush;
        return false;
      }
      if (*r_value != e_value) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "value = " << *r_value
                  << " and value = " << e_value
                  << " are different for key = " << e_key << ". "
                  << std::flush;
        return false;
      }
    }
    return true;
  }

  virtual bool get_by_pointer_negative() {
    // const V* get(const K& key) const;
    for (auto & key_value : m_negative_table) {
      const K& e_key = key_value.first;
      const V& e_value = key_value.second;
      const V* r_value  = m_test_table.get(e_key);
      if (r_value != nullptr) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "unexpected (key = " << e_key << ", "
                  << "value = " << *r_value << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }
  virtual bool find_positive() {
    // iterator find(const K& key);
    for (auto & key_value : m_positive_table) {
      const K& e_key = key_value.first;
      const V& e_value = key_value.second;
      hashtable_iterator_t found = m_test_table.find(e_key);
      // found = m_test_table.end();  // uncomment to test the test
      if (found == m_test_table.end()) {
         std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "no value for for key = " << e_key << ", "
                  << "expected value = " << e_value << ". "
                  << std::flush;
        return false;
      }
      const K& r_key = found->key();
      V& r_value = found->value();
      // if (r_key == e_key)  // uncomment to test the test
      if (r_key != e_key || r_value != e_value) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << ") "
                  << "doesn't match expected "
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }
  virtual bool find_negative() {
    // iterator find(const K& key);
    for (auto & key_value : m_negative_table) {
      const K& e_key = key_value.first;
      const V& e_value = key_value.second;
      hashtable_iterator_t found = m_test_table.find(e_key);
      if (found != m_test_table.end()) {
        const K& r_key = found->key();
        V& r_value = found->value();
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "unexpected (key = " << r_key << ", "
                  << "value = " << r_value << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }
};

template <typename K, typename V>
class CopyAssignmentTest : public InsertionRetrievalTest<K, V> {
 public:
  explicit CopyAssignmentTest(int intput_size = 1024,
                        const std::string & description = "",
                        const std::string & name = "CopyAssignmentTest") :
      InsertionRetrievalTest<K, V>(intput_size, description, name) {}
  virtual bool run() {
    if (this->m_positive_table.empty()) {
      std::cout << "expected hashtable empty in "
                << __ESR_PRETTY_FUNCTION__ << '\n'
                << std::flush;
    }

    if (!this->add_positive()) {
      std::cout << "Unexpected constructor or assignment behavour. "
                << std::flush;
      return false;
    }

    if (!copy_constructor() || !copy_assignment()) {
      std::cout << "Unexpected constructor or assignment behavour. "
                << std::flush;
       return false;
    }

    return true;
  }

 private:
  bool copy_constructor() {
    esr::Hashtable<K, V> temp(this->m_test_table);
    // Destination more than source
    for (auto& t : temp) {
      auto found = this->m_test_table.find(t.key());
      if (found == this->m_test_table.end()) {
         std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                   << "no value in source table for key = "
                   << t.key() << ". " << std::flush;
        return false;
      }

      const K& r_key = found->key();
      V& r_value = found->value();

      if (r_key != t.key() || r_value != t.value()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << ") "
                  << "doesn't match expected "
                  << "(key = " << t.key() << ", "
                  << "value = " << t.value() << "). "
                  << std::flush;
        return false;
      }
    }
    // Source more than destination
    for (auto& t : this->m_test_table) {
      auto found = temp.find(t.key());
      if (found == temp.end()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "no value in destination table for key = "
                  << t.key() << ". " << std::flush;
        return false;
      }

      const K& r_key = found->key();
      V& r_value = found->value();

      if (r_key != t.key() || r_value != t.value()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << ") "
                  << "doesn't match expected "
                  << "(key = " << t.key() << ", "
                  << "value = " << t.value() << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }

  bool copy_assignment() {
    esr::Hashtable<K, V> temp;
    temp = this->m_test_table;
    // Destination more than source
    for (auto& t : temp) {
      auto found = this->m_test_table.find(t.key());
      if (found == this->m_test_table.end()) {
         std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                   << "no value in source table for key = "
                   << t.key() << ". " << std::flush;
        return false;
      }

      const K& r_key = found->key();
      V& r_value = found->value();

      if (r_key != t.key() || r_value != t.value()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << ") "
                  << "doesn't match expected "
                  << "(key = " << t.key() << ", "
                  << "value = " << t.value() << "). "
                  << std::flush;
        return false;
      }
    }
    // Source more than destination
    for (auto& t : this->m_test_table) {
      auto found = temp.find(t.key());
      if (found == temp.end()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "no value in destination table for key = "
                  << t.key() << ". " << std::flush;
        return false;
      }

      const K& r_key = found->key();
      V& r_value = found->value();

      if (r_key != t.key() || r_value != t.value()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << r_key << ", "
                  << "value = " << r_value << ") "
                  << "doesn't match expected "
                  << "(key = " << t.key() << ", "
                  << "value = " << t.value() << "). "
                  << std::flush;
        return false;
      }
    }
    return true;
  }
};

template <typename K, typename V>
class DeletionTest : public InsertionRetrievalTest<K, V> {
 public:
  explicit DeletionTest(int intput_size = 1024,
                        const std::string & description = "",
                        const std::string & name = "DeletionTest") :
      InsertionRetrievalTest<K, V>(intput_size, description, name) {}
  virtual bool run() {
    // if (InsertionRetrievalTest<K, V>::m_positive_table.empty()) {
    if (this->m_positive_table.empty()) {
      std::cout << "expected hashtable empty in "
                << __ESR_PRETTY_FUNCTION__ << '\n'
                << std::flush;
    }
    // bool add(const K& key, const V& value);
    if (!this->add_positive()) {
      std::cout << "Unexpected insertion behavour. " << std::flush;
      return false;
    }

    if (!rm_positive() || !rm_negative()) {
      std::cout << "Unexpected deletion behavour. " << std::flush;
      return false;
    }
    return true;
  }

 private:
  bool rm_positive() {
    esr::Hashtable<K, V> temp = this->m_test_table;
    for (auto& expect : this->m_test_table) {
      temp.remove(expect.key());
      auto found = temp.find(expect.key());
      if (found != temp.end()) {
        std::cout << __ESR_PRETTY_FUNCTION__ << ' '
                  << "(key = " << found->key() << ", "
                  << "value = " << found->value() << ") "
                  << "found after removal. "
                  << std::flush;
        return false;
      }
    }
    return true;
  }

  bool rm_negative() {
    esr::Hashtable<K, V> temp = this->m_test_table;
    for (auto& expect : this->m_negative_table) {
      temp.remove(expect.first);
    }
    return true;
  }
};

}  // namespace esr_test

#endif  // ESR_HASHTEST_FLYMAKE_HPP_
