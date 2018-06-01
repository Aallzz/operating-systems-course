#include "reentrancy_writer.h"

namespace reentrancy_io {

const char reentrancy_writer::mp [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

reentrancy_writer::reentrancy_writer(int const fd) : fd(fd) {};

reentrancy_writer::reentrancy_writer(std::string_view const& file_name) : owner(true) {
  fd = open(file_name.data(), O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
}

reentrancy_writer const& reentrancy_writer::write(std::string_view const& text) const {
  int n {};
  int pos {};
  while ((n = ::write(fd, text.data() + pos, text.size() - pos))) {
    if (n < 0) {
      throw std::runtime_error(strerror(errno));
    }
    pos += n;
    if (pos >= text.size()) {
      break;
    }
  }
  return *this;
}

reentrancy_writer::~reentrancy_writer() {
  if (owner) {
    close(fd);
  }
}

}
