#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <set>
#include <sstream>

#if defined (__linux)
#include "lib/my_epoll.h"
#include <optional>
#include "lib/thread_pool.h"
#else
#include "sys/event.h"
#endif

using namespace std;

#if defined (__linux)
struct server {

  using sa = struct sockaddr;

  constexpr static int max_connections {10};
  
  server(int port) : port(port) {

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw std::runtime_error(std::string("Socket error: ") + strerror(errno));
    }

    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, reinterpret_cast<sa*>(&servaddr), sizeof(servaddr)) < 0) {
      throw std::runtime_error(std::string("Bind error: ") + strerror(errno));
    }

    set_nonblocking(listenfd);

    if (listen(listenfd, max_connections) < 0) {
      throw std::runtime_error(std::string("Listen error: ") + strerror(errno));
    }
  }

  void run() {
    e.add(listenfd, EPOLLIN);
    std::optional<std::vector<epoll_event>> evs;
    while (evs = e.wait()) {
      for (auto const& v : evs.value()) {
        executor.submit([this](auto v) {
          if (v.data.fd == listenfd) {
            int fd;
            if ((fd = accept(listenfd, nullptr, nullptr)) < 0) {
              std::cerr << std::string("Accept error") + strerror(errno) << std::endl;
              return;
            }
            set_nonblocking(fd);
            try {
              e.add(fd, EPOLLIN);
              std::lock_guard<std::mutex> lck(mtx);
              fds.insert(fd);
            } catch (std::exception const& ex) {
              std::cerr << ex.what() << std::endl;
            }
          } else {
            char buffer[buffsz];
            int rcv = recv(v.data.fd, buffer, buffsz, MSG_NOSIGNAL);
            if (rcv == 0 && errno != EAGAIN) {
              shutdown(v.data.fd, SHUT_RDWR);
              close(v.data.fd);
            } else if (rcv > 0) {
              std::lock_guard<std::mutex> lck(mtx);
              std::ostringstream ss;
              ss << std::this_thread::get_id();
              std::string s = "\n"s + ss.str();
              for (auto const& x : fds) {
                if (x == v.data.fd) continue;
                send(x, buffer, rcv, MSG_NOSIGNAL);
                send(x, s.c_str(), s.length(), MSG_NOSIGNAL); 
              }
            }
          }
        }, v);
      }
    }
  }

private:
  pied_piper::my_epoll e{0};
  int listenfd;
  std::set<int> fds;
  
  pied_piper::thread_pool executor;
  std::mutex mtx;
  int port;

  constexpr static int buffsz = 4096;

  int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
      flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }
};
#else

struct server {

  using sa = struct sockaddr;

  constexpr static int max_connections {10};

  server(int port) : port(port) {

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw std::runtime_error(std::string("Socket error: ") + strerror(errno));
    }

    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(listenfd, reinterpret_cast<sa*>(&servaddr), sizeof(servaddr)) < 0) {
      throw std::runtime_error(std::string("Bind error: ") + strerror(errno));
    }

    set_nonblocking(listenfd);

    if (listen(listenfd, max_connections) < 0) {
      throw std::runtime_error(std::string("Listen error: ") + strerror(errno));
    }
  }

  void run() {
    if ((kqueuefd = kqueue()) < 0) {
      throw std::runtime_error(std::string("Kqueue error: ") + strerror(errno));
    }

    struct kevent hv[max_connections + 1];
    for (int i = 0; i < max_connections + 1; ++i) {
      bzero(&hv[i], sizeof(hv[i]));
      EV_SET(&hv[i], listenfd, EVFILT_READ, EV_ADD, 0, 0, 0);
    }

    kevent(kqueuefd, hv, max_connections + 1, NULL, 0, NULL);
    while (true) {
      bzero(hv, sizeof(hv));
      if (kevent(kqueuefd, nullptr, 0, hv, max_connections + 1, nullptr) < 0) {
        std::cerr << std::string("kevent problem: ") + strerror(errno) << std::endl;
        continue;
      }

      for (int i = 0; i < max_connections + 1; ++i) {
        if (hv[i].filter != EVFILT_READ) continue;
        if (hv[i].ident == listenfd) {
          int fd;
          if ((fd = accept(listenfd, nullptr, nullptr)) < 0) {
            std::cerr << std::string("Accept error") + strerror(errno) << std::endl;
            continue;
          }
          set_nonblocking(fd);
          bzero(&hv[i], sizeof(struct kevent));
          EV_SET(&hv[i], fd, EVFILT_READ, EV_ADD, 0, 0, 0);
          if (kevent(kqueuefd, &hv[i], 1, nullptr, 0, nullptr) < 0) {
            throw std::runtime_error(std::string("Kevent error: ") + strerror(errno));
	  }
	  fds.insert(fd);
        } else {
          char buffer[buffsz];
          int rcv = recv(hv[i].ident, buffer, buffsz, 0);
          if (rcv == 0 && errno != EAGAIN) {
            shutdown(hv[i].ident, SHUT_RDWR);
            close(hv[i].ident);
          } else if (rcv > 0) {
            for (auto const& x : fds) {
              if (x == hv[i].ident) continue;
              send(x, buffer, rcv, 0);
            }
          }
        }
      }
    }

  }

private:
  int kqueuefd;
  int listenfd;
  std::set<int> fds;

  int port;

  constexpr static int buffsz = 4096;

  int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
      flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }
};


#endif

int main(int argc, char* argv[]) {
  server srv(atoi(argv[1]));
  srv.run();
  return 0;
}

