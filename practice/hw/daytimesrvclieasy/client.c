#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXLINE    5000
#define SA struct sockaddr

char *arg; 

void* fun1(void* pppp) {
  int socketfd, n;
  char recv[MAXLINE + 1];
  struct sockaddr_in serveraddr;
  
  if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket_error");
    return NULL;
  }
  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(1337);
  
  printf("s = %s\n", arg);

  if (inet_pton(AF_INET, arg, &serveraddr.sin_addr) <= 0) {
    printf("inet_pton error for %s\n", arg);
    return NULL;
  }
  
  if (connect(socketfd, (SA*)&serveraddr, sizeof(serveraddr)) < 0) {
    perror("connect error");
    return NULL;
  }
  
  while ((n = read(socketfd, recv, MAXLINE)) > 0) {
    recv[n] = 0;
    if (fputs(recv, stdout) == EOF) {
      perror("fputs error");
    }
  }

  return NULL;
}

void* fun2(void* a) {
  printf("Hello\n");
  usleep(10000);
  printf("bye\n");
  return 0;
}

int main(int argc, char* argv[]) {
  
  arg = strdup(argv[1]);
  pthread_t id1;
  pthread_t id2;
  
  pthread_create(&id2, NULL, fun2, (void*)argv);
  pthread_create(&id1, NULL, fun1, &argc);
  
  pthread_join(id1, NULL);
  pthread_join(id2, NULL);
  free(arg);
}

