#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static void my_alarm(int signo) {
  signal(SIGALRM, my_alarm);
  alarm(1);
  struct passwd  *roorptr;
  printf("asdfasdfasdfC\n");
  if ((roorptr = getpwnam("root")) == NULL) {
    printf("A\n");
    perror("getpwnam error");
    return ;
  }
  printf("B\n");
}

int main(void) {
  struct passwd *ptr;

  signal(SIGALRM, my_alarm);
  alarm(1);
  for (;;) {
    if ((ptr = getpwnam("alexander")) == NULL) {
      perror("getpwn error");
      return 0;
    }
    if (strcmp(ptr->pw_name, "alexander") != 0) {
      printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
    }
  }
}
