#include "circle_pattern.h"


extern "C" void print_circle(FILE* out, int x) {
  fprintf(out, "%s\033[F\033[F", circle[x % 4]);
  fflush(out);
  usleep(120000);
};

