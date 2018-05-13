#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "lib/my_utility.h"

int main(int argc, char* argv[]) {
 
  struct string_buffer buffer = create_buffer();
  int line_sz;
  int exe_code;
  char **arguments;

  for( ; ; ) {
    clear_buffer(&buffer);
    printf("promt>");
    fflush(stdout);

    if ( read_line_buffer(&buffer, stdin) < 0) {
      break;
    }

    if (strcmp(buffer.buf, "exit\n") ==  0) {
      break;    
    }

    line_sz = 0;
    arguments = NULL;
    split_buffer(&buffer, &arguments, &line_sz);
    
    pid_t pid = fork();
    
    if (pid < 0) {
      perror("fork");
      exit(1);
    }
    
    if (pid == 0) {
      if ( (exe_code = execvp(arguments[0], arguments)) < 0) {
        perror("Error during executing command");
        fflush(stdout);
        exit(errno);
      }
      exit(0);
    } else {
      int status;
      while (wait(&status) != pid);
      printf("\ncommand \'%s\' exited with code %d\n", arguments[0], status);   
      if (arguments != NULL) {
        for (int i = 0; i < line_sz; ++i) {
          free(arguments[i]);
        }
        free(arguments);
      }
    } 

  }
  clear_buffer(&buffer);
  return 0;
}
