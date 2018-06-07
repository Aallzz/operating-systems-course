#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

struct client {

  using sa = struct sockaddr;

  client(std::string const& address, int port) 
    : address(address), port(port) {
    
    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw std::runtime_error(std::string("socket error: ") + strerror(errno));
    }
    
    struct sockaddr_in clientaddr;

    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, address.c_str(), &clientaddr.sin_addr) < 0) {
      throw std::runtime_error(std::string("inet_pton error: ") + strerror(errno));
    }

    if (connect(clientfd, reinterpret_cast<sa*>(&clientaddr), sizeof(clientaddr)) < 0) {
      throw std::runtime_error(std::string("connect error: ") + strerror(errno));
    }
  }

  void run() {
    char buffer[buffsz];
    char buffer2[buffsz];
    
    struct pollfd hv[2];
    hv[0].fd = clientfd;
    hv[0].events = POLLIN;

    hv[1].fd = STDIN_FILENO;
    hv[1].events = POLLIN;

    while (true) {

      bzero(buffer, buffsz);
      bzero(buffer2, buffsz);
      
      poll(hv, 2, -1);
      if ((hv[1].revents & POLLIN) == POLLIN) {
        std::string s;
        std::getline(std::cin, s);
        write(clientfd, s.c_str(), s.length());
      }
      if ((hv[0].revents & POLLIN) == POLLIN) {
        read(clientfd, buffer2, buffsz);
        printf("Received: %s\n", buffer2);
      }
    }
  }

private:
  
  int clientfd;
  std::string address;
  int port;

  constexpr static int buffsz = 4096;
};

int main(int argc, char* argv[]) {
  client c(argv[1], atoi(argv[2]));
  c.run();
  return 0;
}
