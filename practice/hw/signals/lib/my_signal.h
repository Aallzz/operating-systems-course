#ifndef MY_SIGNAL_LIB
#define MY_SIGNAL_LIB

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>

#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

namespace signal_helper {

struct signals {
  
  static const std::map<std::string, int> flags_name;

  using handler_type = void(*)(int);

  using sa_handler_type = void(*)(int, siginfo_t*, void*);

  signals();
  
  signals& empty_set();

  signals& fill_set();

  signals& add_set(int const signo);

  signals& del_set(int const signo);
  
  signals& set_handler(handler_type handler);

  signals& set_handler(sa_handler_type handler);
  
  signals& add_flag(int const flag);

  signals& del_flag(int const flag);

  bool is_member_set(int const signo) const;
  
  static signals get_blocked_signals();

  static signals get_pending_signals();

  std::vector<int> list_int_signals_set() const;

  std::vector<std::string> list_string_signals_set() const;
  
  signals set_as_process_signal_mask(int const how) const;
  
  signals set_for_signal(int const signo) const; 
  
  static signals factory();
  
  void suspend() const; 
  
  static int setjmp(int val);

  static void longjmp(int val);

private:
 
 struct sigaction act;
 
 static sigjmp_buf jmpbuf[15];
 static volatile sig_atomic_t canjump[15];

 signals(const sigset_t signals_set);
 
 signals(const struct sigaction newact);

 template<typename F, typename... Args>
 signals& sigsetoperation(F f, Args&&... args) {
   if (f(std::forward<Args>(args)...) < 0) {
     throw std::runtime_error(strerror(errno));
   }
   return *this;
 }

};
}

#endif

