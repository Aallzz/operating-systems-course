#include "my_utility.h"

#include <iostream>

//extern "C" void split_buffer(struct string_buffer const * const, char***, int*);  

std::vector<std::string> split_string(std::string const& str) {
  string_buffer buf;
  char** tokens = NULL;
  int tokens_cnt = 0;

  buf.buf = strdup(str.c_str());
  buf.sz = strlen(str.c_str());
    
  split_buffer(&buf, &tokens, &tokens_cnt);

  std::vector<std::string> res;
  res.reserve(tokens_cnt);

  for (int i = 0; i < tokens_cnt - 1; ++i) {
    res.push_back(std::string(tokens[i]));
    free(tokens[i]);
  }
  free(tokens);
  clear_buffer(&buf);
  return res;

};

