#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef MY_UTILITY
#define MY_UTILITY 
#define MX_SPLIT_RESULT_SIZE 100

struct string_buffer{
  char* buf;
  size_t sz;
};

struct string_buffer create_buffer(); 
void clear_buffer(struct string_buffer* buffer);
int read_line_buffer(struct string_buffer* buffer, FILE* from);
void split_buffer(struct string_buffer const * const buffer, char*** res, int* sz);

int has_close_quote(char const * const string);
int has_open_quote(char const * const string);

#endif 
