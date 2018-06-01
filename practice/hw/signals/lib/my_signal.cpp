#include "my_signal.h"

namespace signal_helper {

const std::map<std::string, int> signals::flags_name {
#ifdef SA_INTERRUPT 
  {"SA_SIGINFO"   ,   SA_SIGINFO},
#endif
  {"SA_NOCLDSTOP" ,   SA_NOCLDSTOP},
  {"SA_NOCLDWAIT" ,   SA_NOCLDWAIT},
  {"SA_NODEFER"   ,   SA_NODEFER},
  {"SA_ONSTACK"   ,   SA_ONSTACK},
  {"SA_RESETHAND" ,   SA_RESETHAND},
  {"SA_RESTART"   ,   SA_RESTART},
  {"SA_SIGINFO"   ,   SA_SIGINFO}
};

sigjmp_buf signals::jmpbuf[15];
volatile sig_atomic_t signals::canjump[15];

signals& signals::empty_set() {
  return sigsetoperation(sigemptyset, &act.sa_mask); 
}

signals& signals::fill_set() {
  return sigsetoperation(sigfillset, &act.sa_mask); 
}

signals& signals::add_set(int const signo) {
  return sigsetoperation(sigaddset, &act.sa_mask, signo); 
}
  
signals& signals::del_set(int const signo) {
  return sigsetoperation(sigdelset, &act.sa_mask, signo); 
}

signals& signals::set_handler(handler_type handler) {
  act.sa_handler = handler;
  act.sa_flags &= ~SA_SIGINFO;
  return *this;
}

signals& signals::set_handler(sa_handler_type handler) {
  act.sa_sigaction = handler;
  act.sa_flags |= SA_SIGINFO;
  return *this;
}

signals& signals::add_flag(int const flag) {
  if (flag == SA_SIGINFO) {
    throw std::runtime_error("SA_SIGINFO added automaticaly if need");
  }
  act.sa_flags |= flag; 
  return *this;
}

signals& signals::del_flag(int const flag) {
  if (flag == SA_SIGINFO) {
    throw std::runtime_error("SA_SIGINFO removed automaticaly if need");
  }
  act.sa_flags &= ~flag;
  return *this;
}

bool signals::is_member_set(int const signo) const {
  int res {};
  if ((res = sigismember(&act.sa_mask, signo)) < 0) {
    throw std::runtime_error(strerror(errno));
  }
  return res;
}
  
signals signals::get_blocked_signals() {
  using namespace std::string_literals;
  sigset_t cur;
  if (sigprocmask(0, NULL, &cur) < 0) {
    throw std::runtime_error("Error in getting blocked signals: "s + strerror(errno));
  }
  return signals(cur);
}

signals signals::get_pending_signals() {
  using namespace std::string_literals;
  sigset_t cur;
  if (sigpending(&cur) < 0) {
    throw std::runtime_error("Error in getting pending signals: "s + strerror(errno));
  }
  return signals(cur);
}

signals signals::factory() {
  return signals();
}

signals signals::set_for_signal(int const signo) const {
  using namespace std::string_literals;
  struct sigaction old;
  if (sigaction(signo, &act, &old) < 0) {
    throw std::runtime_error("Error in settin sigaction for signal"s + std::to_string(signo) + ": "s + strerror(errno));
  }
  return signals(old);
}

std::vector<int> signals::list_int_signals_set() const  {
  std::vector<int> res;
  for (int i = 1; i < 64; ++i) {
    if (is_member_set(i)) {
      res.push_back(i);
    }
  }
  return res;
}

std::vector<std::string> signals::list_string_signals_set() const {
  auto vec = list_int_signals_set();
  std::vector<std::string> res;
  std::transform(vec.begin(), vec.end(), std::back_inserter(res), [](int x) {
        return std::string(strsignal(x));
      });
  return res;
}

signals signals::set_as_process_signal_mask(int const how) const {
  using namespace std::string_literals;
  sigset_t old;
  if (sigprocmask(how, &act.sa_mask, &old) < 0) {
    throw std::runtime_error("Error in setting process blicked signals: "s + strerror(errno));
  }
  return signals(old);
}

void signals::suspend() const {
  using namespace std::string_literals;
  sigsuspend(&act.sa_mask);      // atomic setprocmask + pause
}

signals::signals() {
  act.sa_flags = 0;
  empty_set();
  act.sa_handler = nullptr;
}

int signals::setjmp(int val) {
  int res = sigsetjmp(jmpbuf[val], 0);
  if (res == 0) {
    canjump[val] = 1;
  }
  return res;
}

void signals::longjmp(int val) {
  if (canjump[val] == 0) {
    return;
  }
  canjump[val] = 0;
  siglongjmp(jmpbuf[val], 1);
}

signals::signals(const sigset_t mask) : signals() {
  act.sa_mask = mask;
}

signals::signals(const struct sigaction newact) : act(newact) {}

}
