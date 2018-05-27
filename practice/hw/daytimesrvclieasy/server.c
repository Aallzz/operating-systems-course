#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 5000
#define SA struct sockaddr
#define LISTENQ 100

int main(int argc, char** argv) {
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buff[MAXLINE];
  time_t ticks;
  
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error");
    return 0;
  }

  bzero(&servaddr, sizeof(servaddr)); 
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(1337);

  if (bind(listenfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind error");
    return 0;
  }

  if (listen(listenfd, LISTENQ) < 0) {
    perror("listen error");
    return 0;
  }

  for ( ; ; ) {
    if ((connfd = accept(listenfd, (SA*)NULL, NULL)) < 0) {
      perror("accept error");
      return 0;
    }

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    if (write(connfd, buff, strlen(buff)) < 0) {
      perror("write error");
      return 0;
    }
  }
  return 0;
}

