#include "memory_manager.h"

std::size_t memory_manager::clac_real_size(std::size_t const sz) {
  std::size_t page_size = sysconf(_SC_PAGE_SIZE);
  std::size_t cnt = (sz + page_size - 1) / page_size;
  return page_size * cnt;
};

void* memory_manager::map(std::size_t sz, int filedis, std::size_t offset) {
  void* result;
  if ((result = mmap(nullptr, sz * sizeof(char), 
      PROT_READ | PROT_WRITE,
      MAP_ANONYMOUS | MAP_PRIVATE, 
      filedis, offset)) == (void*)(-1)) {
    perror("Error during map operation");
    throw std::exception();    
  } else {
    return result;
  }
};

void memory_manager::unmap(void* addr, size_t sz) {
  int result;
  if ((result = munmap(addr, sz)) != 0) {
    perror("Error during unmap operation");
    throw std::exception();
  }
}

void memory_manager::protect(void* addr, size_t sz, int prot) {
  int result;
  if ((result = mprotect(addr, sz, prot)) != 0) {
    perror("Error during protect operation");
    throw std::exception();
  }
}
