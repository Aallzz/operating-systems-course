#ifndef MY_JIT_UTILITY
#define MY_JIT_UTILITY

extern "C" {
  #include "shelllib/my_utility.h"
}

#include <string>
#include <vector>



std::vector<std::string> split_string(std::string const& str);

#endif

