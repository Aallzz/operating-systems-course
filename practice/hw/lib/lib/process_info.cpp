#include "process_info.h"

namespace process { 

  process_info::process_info() {
    pid = getpid();
    update_values();
  };

  void process_info::update() {
    update_values();
  }

  std::size_t process_info::get_info(part_info part) const {
    return values[static_cast<int>(part)] * sysconf(_SC_PAGE_SIZE);
  }

  void process_info::update_values() {
    std::ifstream reader("/proc/self/statm");
    for (auto& x : values) {
      reader >> x;
    }
    reader.close();
  }
}
