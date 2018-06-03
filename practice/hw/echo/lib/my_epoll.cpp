#include "my_epoll.h"

namespace pied_piper {

using namespace std::string_literals;

  my_epoll_error::my_epoll_error(const char* msg) : std::runtime_error(msg) {};
  
  my_epoll_error::my_epoll_error(std::string const& msg) : std::runtime_error(msg) {};

  const char* my_epoll_error::what() const noexcept {
    return std::runtime_error::what();
  }

  my_epoll::my_epoll(int flag) {
    if ((epoll_fd = epoll_create1(flag)) < 0) {
      throw my_epoll_error("Cannot create epoll: "s + strerror(errno));
    }
  }
  
  void my_epoll::add(int filedis, int events) {
    epoll_operation(filedis, events, EPOLL_CTL_ADD, 
        "Cannot add to epoll: ");
  }

  void my_epoll::modify(int filedis, int events) {
    epoll_operation(filedis, events, EPOLL_CTL_MOD,
        "Cannot make modification to epoll: ");
  }
  
  void my_epoll::del(int filedis) {
    epoll_operation(filedis, 0, EPOLL_CTL_MOD,
        "Cannot delete from epoll: ");
  }

  std::optional<std::vector<epoll_event>> my_epoll::wait() {
    int ready;
    struct epoll_event evs[maximum_epoll_events];
    if ((ready = epoll_wait(epoll_fd, evs, maximum_epoll_events, -1)) < 0) {
      return std::optional<std::vector<epoll_event>>();
    } else {
      return std::vector<epoll_event>(evs, evs + sizeof(evs) / sizeof(evs[0]));
    }
  }

  my_epoll::~my_epoll() {
    if (close(epoll_fd) < 0) {
      //throw my_epoll_error("Error while closing epoll: "s + strerror(errno));
    }
  }
  
  void my_epoll::epoll_operation(int filedis, int events, int action, std::string const& emsg) {
    struct epoll_event e;
    e.events = events;
    e.data.fd = filedis;
    if (epoll_ctl(epoll_fd, action, filedis, (action == EPOLL_CTL_DEL ? nullptr : &e)) < 0) {
      throw my_epoll_error(emsg + strerror(errno));
    }
  }
}
