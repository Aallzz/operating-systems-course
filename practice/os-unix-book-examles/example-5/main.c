#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char** environ;

void f () {
  printf("endedasdfasdfsafd");
}

int main(int argc, char* argv[]) {

  atexit(f);
  int i;
  printf("parametrs count given to the program %s is %d\n", argv[0], argc - 1);
  for (i = 0; i < argc; ++i) {
    printf("argv[%d] = $=%s\n", i, argv[i]);
  }
  for (i = 0; i < 200; ++i) {
    if (environ[i] != NULL) {
      printf("environ[%d] : %s\n", i, environ[i]);
    } else {
      break;
    }
  }
  
  char *term;
  char buf[200], var[200];
  
  const char *evar = "TERMM";
  if ((term = getenv(evar)) == NULL) {
    printf("Undefined %s variable, enter its value:", evar);
    gets(buf);
    sprintf(var, "%s=%s", evar, buf);
    putenv(var);
    printf("%s\n", var);
  } else {
    printf("%s=%s. Change? [N]", evar, getenv("TERM"));
  }
  return 0;
}
