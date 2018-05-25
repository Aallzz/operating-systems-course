#include "circle_pattern.h"


extern "C" void print_circle(FILE* out, int x) {
  int a[100000];
  for (int i = 0; i < 100000; ++i) {
    a[i] = i + 1;
  }
  fprintf(out, "%s\033[F\033[F", circle[x % 4]);
  fflush(out);
  usleep(120000);
};

