// Copyright 2016
#ifndef ESR_HASHEXCEPT_FLYMAKE_HPP_
#define ESR_HASHEXCEPT_FLYMAKE_HPP_

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef __GNUC__
#define __ESR_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __ESR_PRETTY_FUNCTION__ __func__
#endif
namespace esr {

namespace exception {

using std::string;

class hashtable: public std::out_of_range {
 public:
  explicit hashtable(const string& who_arg = "",
                     const string& what_arg = "bad bucket index") :
      out_of_range(what_arg),
      m_who(who_arg) {}
  const string& who() const { return m_who; }
 protected:
  string m_who;
};

class bucket_index: public hashtable {
 public:
  explicit bucket_index(int index, const string& who_arg = "",
    const string& what_arg = "bad bucket index") :
      hashtable(who_arg, what_arg), m_index(index) {}
      size_t index() const { return m_index; }
 private:
     size_t m_index;
};

class end_iterator: public hashtable {
 public:
  explicit end_iterator(int index, const string& who_arg = "",
    const string& what_arg = "dereferencing end iterator") :
      hashtable(who_arg, what_arg), m_index(index) {}
      size_t current_bucket_index() const { return m_index; }
 private:
     size_t m_index;
};



  
  /*
  class bucket_index: public hashtable {
 public:
  explicit bucket_index(int index, const string& who_arg = "",
    const string& what_arg = "bad bucket index") :
      hashtable(who_arg, what_arg), m_index(index) {}
 private:
  size_t m_index;
};
  */
}

}


/*

class bad_bucket_index: public std::out_of_range {
 public:
  explicit bucket_index_error(int index,
                              const string& where_arg = "",
                              const string& what_arg = "bad bucket index") :
      out_of_range(what_arg),
      m_index(index),
      m_where(where_arg) {}
 private:
  int m_index;
  string m_where;
};

class deref_end_iterator: public std::out_of_range {
 public:
  explicit deref_end_iterator(int index,
                              const string& where_arg = "",
                              const string& what_arg = "bad bucket index") :
      out_of_range(what_arg),
      m_index(index),
      m_where(where_arg) {}
 private:
  int m_index;
  string m_where;
};
*/

#endif  // ESR_HASHEXCEPT_FLYMAKE_HPP_
