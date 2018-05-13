#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
  fprintf(stderr, "ENOMEM: %s\n", strerror(ENOMEM));
  errno = ENOEXEC;
  perror(argv[0]);
}
