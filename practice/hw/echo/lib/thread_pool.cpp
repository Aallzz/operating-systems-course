#include "thread_pool.h"

namespace pied_piper {

thread_pool::thread_pool() 
  : thread_pool(std::max(2u, std::thread::hardware_concurrency() - 1)) 
{};

thread_pool::thread_pool(size_t threads_count) {
  for (int i = 0; i < threads_count; ++i) {
    workers.emplace_back([this] {
      while (!done) {
        try {
          (*queue.get(200ms))();
        } catch (...) {
          //std::this_thread::yield();
        }
      }
    });
  };
}

thread_pool::~thread_pool() {
  done = true;
  for_each(workers.begin(), workers.end(), std::mem_fn(&std::thread::join));
}

void thread_pool::stop() {
  done = true;
}

}
