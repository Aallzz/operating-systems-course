#ifndef MY_THREAD_POOL_H
#define MY_THREAD_POOL_H

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <type_traits>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <future>

#include <iostream>

#include "blocking_queue.h"

namespace pied_piper {

  using namespace std::chrono_literals;

  struct thread_pool {

    thread_pool();
    thread_pool(size_t threads_count);

    template<typename FF, typename... AArgs>
    auto submit(FF&& f, AArgs&&... args) {
      //using result_type = std::invoke_result_t<FF(AArgs...)>;
      using result_type = typename std::result_of<FF(AArgs...)>::type;
      auto task = std::make_shared<std::packaged_task<result_type()>>(
          std::bind(std::forward<FF>(f), std::forward<AArgs>(args)...));
      std::future<result_type> result = task->get_future();
      try {
        queue.push([task](){(*task)();}, 100s);
      } catch (std::exception& e) {
        throw e;
      }
      return result;
    };

    ~thread_pool();
    
    void stop();

  private:
    
    std::vector<std::thread> workers;
    sync_queue<std::function<void()>> queue; 
    std::atomic_bool done {false};

  };

}

#endif

