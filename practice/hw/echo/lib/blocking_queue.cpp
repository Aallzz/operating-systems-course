#include "blocking_queue.h"

sync_queue_exception::sync_queue_exception(const char* msg) 
  : std::runtime_error(msg) {};

sync_queue_exception::sync_queue_exception(std::string const& msg) 
  : std::runtime_error(msg) {};

const char* sync_queue_exception::what() const noexcept {
 return std::runtime_error::what(); 
}

