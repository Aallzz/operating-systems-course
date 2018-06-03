#ifndef MY_BLOCKING_QUEUE_H
#define MY_BLOCKING_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <exception>
#include <memory>


#include <iostream>

struct sync_queue_exception : std::runtime_error {
  
  sync_queue_exception(const char* msg);
  
  sync_queue_exception(std::string const& msg);

  const char* what() const noexcept override;
};

template<typename T>
struct sync_queue {
  
  sync_queue() = default;
    
  sync_queue(int new_max_queue_len) : max_queue_len(new_max_queue_len) {};
  
  template<typename TT>
  void push(TT&& value, std::chrono::steady_clock::duration d) {
    std::unique_lock<std::mutex> lck(mtx);
    int res = cnd.wait_for(lck, d, [this] {return q.size() < max_queue_len;});
    if (res) {
      q.push(std::forward<TT>(value));
      cnd.notify_one();
    } else {
      throw sync_queue_exception("Sync queue: push() timeout");
    }
  }

  template<typename... Args>
  void emplace(Args&&... args) {
    std::unique_lock<std::mutex> lck(mtx);
    q.emplace(std::forward<Args>(args)...);
    cnd.notify_one();
  }

  std::shared_ptr<T> get(std::chrono::steady_clock::duration d) {
    std::unique_lock<std::mutex> lck(mtx);
    if (q.empty()) {
     // std::cout << "need" << std::endl;
    }
    int res = cnd.wait_for(lck, d, [this] {return !q.empty();});
    if (res) {
      std::shared_ptr<T> const value (std::make_shared<T>(q.front()));
      q.pop();
      return value;
    } else {
      throw sync_queue_exception("Sync_queue: get() timeout");
    }
  }

  void get(T& value, std::chrono::steady_clock::duration d) {
    std::unique_lock<std::mutex> lck(mtx);
    int res = cnd.wait_for(lck, d, [this] {return !q.empty();});
    if (res) {
      value = q.front();
      q.pop();
    } else {
      throw sync_queue_exception("Sync_queue: get() timeout");
    }
  }
  
  friend void swap(sync_queue& a, sync_queue& b) {
    std::lock(a.mtx, b.mtx);
    std::lock_guard<std::mutex> lck_a(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lck_b(b.mtx, std::adopt_lock);
    std::swap(a.q, b.q);
  }

private:
  
  int max_queue_len {10000};
  mutable std::mutex mtx;
  std::condition_variable cnd;
  std::queue<T> q;
};

#endif
