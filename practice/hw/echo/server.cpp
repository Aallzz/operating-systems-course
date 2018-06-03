#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <exception>
#include <stdexcept>
#include <string>
#include <optional>
#include <chrono>
#include <iomanip>
#include <map>
#include <list>
#include <optional>

#include "lib/my_epoll.h"
#include "lib/thread_pool.h"

namespace pied_piper {
struct server_error : std::runtime_error {

  server_error(char const* msg) : std::runtime_error(msg) {};
  
  server_error(std::string const& msg) : std::runtime_error(msg) {};

  const char* what() const noexcept {
    return std::runtime_error::what();
  }
};

using namespace std::string_literals;

struct server {
  
  using sa = struct sockaddr;

  static constexpr int maximum_connections {10};
  static constexpr size_t maximum_buffer_size {5000};

  explicit server(int port) {
    struct sockaddr_in servaddr;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
      throw server_error("Cannot open socket: "s + strerror(errno));
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    fcntl(listenfd, F_SETFL, fcntl(listenfd, F_GETFL, 0) | O_NONBLOCK);
    
    if (bind(listenfd, reinterpret_cast<sa*>(&servaddr), sizeof(servaddr)) < 0) {
      throw server_error("Cannot bind: "s + strerror(errno));
    }

    if (listen(listenfd, maximum_connections) < 0) {
      throw server_error("Cannot mark socket as listen socket"s + strerror(errno));
    }

    e.add(listenfd); 
  }
  
  void start() {
    std::optional<std::vector<epoll_event>> evs;
    while ((evs = e.wait())) {
      for (auto const& v : evs.value()) {
        executor.submit([this](int fd){
              try {
                if (listenfd == fd) {
                  add_client(fd);
                  send(fd, get_stime());
                } else {
                  auto r = recieve(fd);
                  if (r.empty()) return;
                  std::lock_guard<std::mutex> lck(cmtx);
                  for (auto const& c : clients) {
                    send(c, r + ": " + get_stime());
                  }
                }
              } catch(std::exception& e) {
                std::cout << e.what() << std::endl;
              }
            },
            v.data.fd);
      }
    }
    throw server_error("Stop server: "s + strerror(errno));
  }

  
  void close() {
    ::close(listenfd);
  }

private:
  int listenfd;
  thread_pool executor {};
  my_epoll e {EPOLL_CLOEXEC};
  std::map<int, std::string> snd; 
  std::mutex mtx;

  std::list<int> clients;
  std::mutex cmtx;

  void add_client(int fd) {
    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(struct sockaddr_in);
    if ((clientfd = accept(listenfd, reinterpret_cast<sa*>(clientaddrlen), &clientaddrlen)) < 0) {
      throw server_error("Cannot add new client "s 
          + inet_ntoa(clientaddr.sin_addr) + ":" 
          + std::to_string(clientaddr.sin_port) + " :" + strerror(errno));
    }
    fcntl(clientfd, F_SETFL, fcntl(clientfd, F_GETFL, 0) | O_NONBLOCK);
    e.add(clientfd);
    std::lock_guard<std::mutex> lck(cmtx);
    clients.push_back(clientfd);
  }
 
  std::string get_stime() const {
    time_t ticks;
    char buffer[maximum_buffer_size];
    ticks = time(NULL);
    snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
    return buffer;
  }

  void send(int fd, std::string const& msg) {
    std::lock_guard<std::mutex> lck(mtx); 
    int len;
    snd[fd].append(msg);
    if ((len = write(fd, snd[fd].c_str(), snd[fd].length())) < 0) {
      throw server_error("Cannot send response to client: "s + strerror(errno)); 
    }
    if (!len) {
      snd.erase(fd);
      return ;
    } else {
      snd[fd] = snd[fd].substr(len);
    }
  }

  std::string recieve(int fd) {
    char buffer[maximum_buffer_size];
    int len;
    if ((len = recv(fd, buffer, sizeof(buffer), 0)) < 0) {
      throw server_error("Error during recv"s + strerror(errno));
    }
    if (len == 0 && errno != EAGAIN) {
      std::lock_guard<std::mutex> lck(cmtx);
      clients.remove(fd);
      e.del(fd);
      ::close(fd);
      throw server_error("Closed client"s);
    } else {
      return buffer; 
    }
  };
};
}

int main() {
  pied_piper::server s(8888);
  s.start();
  return 0;
}
