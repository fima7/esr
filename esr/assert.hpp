// Copyright 2016
#ifndef ESR_ESR_ASSERT_FLYMAKE_HPP_
#define ESR_ESR_ASSERT_FLYMAKE_HPP_

// noexception
#include <iostream>

#ifdef __GNUC__
#define __ESR_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __ESR_PRETTY_FUNCTION__ __func__
#endif

// noexception
inline void _assert(const char* expression,
                    const char* function,
                    const char* file,
                    int line) {
  // noexception
  std::cerr << '\n';
  std::cerr << "Assertion: " << expression
            << " failed in " << function
            << " at " << file << ", " << line
            << '\n';
  abort();
}

#endif  // ESR_ESR_ASSERT_FLYMAKE_HPP_
/*
void func(const char* expression,
          const char* function,
          const char* file,
          int line) {
  std::cerr << "Assertion: " << expression
            << " failed in " << function
            << " at " << file << ", " << line;
}

template<typename Arg1, typename... Args>
void func(const char* expression,
          const char* function,
          const char* file,
          int line,
          const Arg1& arg1, const Args&... args) {
  func(expression, function, file, line, args...);
  std::cerr << "; " << arg1;
}
*/
