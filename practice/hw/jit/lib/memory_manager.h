#ifndef MY_JIT_MEMORY_MANAGER
#define MY_JIT_MEMORY_MANAGER

#include <cstdint>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <exception>

namespace memory_manager {

  std::size_t clac_real_size(std::size_t const sz);
  void* map(std::size_t sz, int filedis, std::size_t offset);
  void unmap(void* addr, size_t sz);
  void protect(void* addr, size_t sz, int prot);

};

#endif
