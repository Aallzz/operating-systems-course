#include "my_utility.h"

struct string_buffer create_buffer() {
  struct string_buffer buf;
  buf.buf = NULL;
  buf.sz = 0;
  return buf;
}

void clear_buffer(struct string_buffer* buffer) {
  free(buffer->buf);
  buffer->sz = 0;
  buffer->buf = NULL;
}

int read_line_buffer(struct string_buffer* buffer, FILE* from) {
  clear_buffer(buffer);
  return getline(&buffer->buf, &buffer->sz, from);
}

void split_buffer(struct string_buffer const * const buffer, char*** res, int* sz) {
  char* temp = strdup(buffer->buf);
  *sz = 0;
  temp[strlen(temp) - 1] = '\0';
  char* token = strtok(temp, " \t");
  int capacity = 1;
  *res = (char**)realloc(*res, sizeof(char*));
  int complex_argument = 0;
  while (token) {
    if (*sz >= capacity - 1) {
      *res = (char**)realloc(*res, (capacity + capacity) * sizeof(char*));
      capacity += capacity;
    }
    if (complex_argument) {
      if (has_close_quote(token)) {
        complex_argument = 0;
      }
      strcat((*res)[*sz - 1], token);
    } else {
      if (has_open_quote(token)) {
        complex_argument = 1;
      }
      (*res)[(*sz)++] = strdup(token);
    }
    token = strtok(NULL, " \t");
  }
  (*res)[(*sz)++] = NULL;
  free(temp);
}

int has_open_quote(char const * const string) {
  int n = strlen(string);
  if (n != 0) {
    return (string[0] == '\"');
  }
  return 0;
}

int has_close_quote(char const * const string) {
  int n = strlen(string);
  if (n == 0 || string[n - 1] != '\"' || string[n - 1] == '\\') 
    return 0;
  return 1;
}
