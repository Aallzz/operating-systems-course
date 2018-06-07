#ifndef MY_EPOLL_H
#define MY_EPOLL_H

#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <stdexcept>
#include <string>
#include <vector>
#include <optional>

namespace pied_piper {

using namespace std::string_literals;

struct my_epoll_error : std::runtime_error {
  
  my_epoll_error(const char* msg);
  my_epoll_error(std::string const& msg);
  const char* what() const noexcept override;

};

struct my_epoll {
  
  static constexpr size_t maximum_epoll_events {30};
  
  explicit my_epoll(int flag);
  void add(int filedis, int events = EPOLLIN | EPOLLET);
  void modify(int filedis, int events = EPOLLIN | EPOLLET);
  void del(int filedis);
  std::optional<std::vector<epoll_event>> wait();
  ~my_epoll();
  
private:
  
  int epoll_fd;
  
  void epoll_operation(int filedis, int events, int action, std::string const& emsg);
};

}
#endif
