#ifndef MY_PROCESS_INFO
#define MY_PROCESS_INFO

#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <array>

namespace process {
  
  enum class part_info {
    total_size,
    resident_size,
    shared_size,
    text_size,
    lib_size,
    data_size,
    dirty_size,
  };

// https://elinux.org/Runtime_Memory_Measurement
struct process_info {

  process_info();
  void update();
  std::size_t get_info(part_info part) const;

private:
  // size in pages
  std::array<std::size_t, 7> values;
  pid_t pid;

  void update_values();
};
}
#endif 
