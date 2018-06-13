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
    

    if((clientfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
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
      if (errno == EINPROGRESS) { 
        connected = false;
      } else 
        throw std::runtime_error(std::string("connect error: ") + strerror(errno));
    }
    
    hv[0].fd = clientfd;
    hv[0].events = POLLIN | POLLOUT;

    hv[1].fd = STDIN_FILENO;
    hv[1].events = POLLIN;

  }

  void run() {
    char buffer[buffsz];
    char buffer2[buffsz];

    while (true) {

      bzero(buffer, buffsz);
      bzero(buffer2, buffsz);
      
      poll(hv, 2, -1);
      if ((hv[1].revents & POLLIN) == POLLIN) {
        if (connected) {
          std::string s;
          std::getline(std::cin, s);
          if ((hv[0].revents & POLLOUT) == POLLOUT) {
            write(clientfd, s.c_str(), s.length());
          }
        }
      }
      if ((hv[0].revents & POLLIN) == POLLIN) {
        if (!connected) { 
          std::cout << "No connectin" << std::endl;
        } else {
          int res = read(clientfd, buffer2, buffsz);
          if (!res) {
            throw std::runtime_error(std::string("Server is terminated"));
          } 
          printf("Received: %s\n", buffer2);
        }
      }
      if ((hv[0].revents & POLLOUT) == POLLOUT) {
        if (!connected) {
          int err {};
          socklen_t len = sizeof(err);
          if (getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) != -1) {
             if (!err) {
              connected = true;
             } else {
              close(clientfd);
              throw std::runtime_error(std::string("Connection error: "));
             }
          }
        }
      }
      
    }
  }

private:
  
  int clientfd;
  std::string address;
  int port;
  
  struct pollfd hv[2];
  bool connected {true};

  constexpr static int buffsz = 4096;
};

int main(int argc, char* argv[]) {
  client c(argv[1], atoi(argv[2]));
  c.run();
  return 0;
}
