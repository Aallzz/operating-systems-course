#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char** argv) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  int pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    printf("Child\n");
  } else { 
    printf("Parent\n");
  }
  gettimeofday(&stop, NULL);
  printf("took %lu\n", stop.tv_usec - start.tv_usec);
  return 0;
}
