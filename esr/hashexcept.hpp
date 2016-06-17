// Copyright 2016
#ifndef ESR_HASHEXCEPT_FLYMAKE_HPP_
#define ESR_HASHEXCEPT_FLYMAKE_HPP_
////////////////////////////////////////////////////////////////////////////////
// Hashtable Exceptions.
////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>  // std::out_of_range
#include <string>     // std::string.

namespace esr {
namespace exception {

using std::string;

////////////////////////////////////////////////////////////////////////////////
/// @class hashtable.
///
/// @brief Hashtable exception.
/// Common type for all Hashtable exceptions.
////////////////////////////////////////////////////////////////////////////////
class hashtable: public std::out_of_range {
 public:
  /// @brief Default constructor, creates Hashtable exception.
  /// @param who_arg is a data to identify function which throws an exception.
  /// @param what_arg is a data describing the exception.
  /// @return nothing.
  explicit hashtable(const string& who_arg = "",
                     const string& what_arg = "bad bucket index") :
      out_of_range(what_arg),
      m_who(who_arg) {}

  /// @brief Gets exception thrower desription.
  /// @return Description provided by function which throws the exception.
  const string& who() const { return m_who; }
 protected:
  string m_who;  //< exception origin.
};

////////////////////////////////////////////////////////////////////////////////
/// @class bucket_index.
///
/// @brief Bucket index exception.
/// Index is out of range of bucket array.
////////////////////////////////////////////////////////////////////////////////
class bucket_index: public hashtable {
 public:
  /// @brief Default constructor, creates a bucket exception.
  /// @param index is a value of bucket index.
  /// @param who_arg is a data to identify function which throws an exception.
  /// @param what_arg is a data describing the exception.
  /// @return nothing.
  explicit bucket_index(int index, const string& who_arg = "",
    const string& what_arg = "bad bucket index") :
      hashtable(who_arg, what_arg), m_index(index) {}

  /// @brief Gets an index of bucket array.
  /// @return index of bucket array.
  size_t index() const { return m_index; }
 private:
  size_t m_index;  //< bucket index.
};

////////////////////////////////////////////////////////////////////////////////
/// @class end_iterator.
///
/// @brief End iterator exception.
/// Attempt to dereference end iterator.
////////////////////////////////////////////////////////////////////////////////
class end_iterator: public hashtable {
 public:
  /// @brief Default constructor, creates an end iterator exception.
  /// @param index is a value of bucket index.
  /// @param who_arg is a data to identify function which throws an exception.
  /// @param what_arg is a data describing the exception.
  /// @return nothing.
  explicit end_iterator(int index, const string& who_arg = "",
                        const string& what_arg = "dereferencing end iterator") :
      hashtable(who_arg, what_arg), m_index(index) {}

  /// @brief Gets index of current bucket.
  /// @return index of current bucket.
  size_t current_bucket_index() const { return m_index; }
 private:
  size_t m_index;  //< bucket index.
};

}  // namespace exception
}  // namespace esr

#ifdef __GNUC__
#define __ESR_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __ESR_PRETTY_FUNCTION__ __func__
#endif

#endif  // ESR_HASHEXCEPT_FLYMAKE_HPP_
