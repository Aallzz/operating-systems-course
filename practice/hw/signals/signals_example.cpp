#include <bits/stdc++.h>
#include "lib/my_signal.h"
#include "lib/reentrancy_writer.h"

using namespace std;
using namespace reentrancy_io;
using namespace signal_helper;

static void sig_quit(int);

int main() {
 
  cout << "Position 1\n";
  if (signal(SIGINT, sig_quit) == SIG_ERR) {
    perror("SMTH");
    return 0;
  }
  cout << "SIGINT MAPPED to sig_quit\n";
  signals sig;
  signals old;
  
  cout << "ESTABLISH BLOCKED SIGNALS MASK\n";
  old = sig.empty_set().add_set(SIGINT).set_as_process_signal_mask(SIG_BLOCK);
  cout << "DONE\n";
  
  sleep(1);
  
  cout << "Getting pending signals\n";
  signals sig2 = signals::get_pending_signals();
  if (sig2.is_member_set(SIGINT)) {
    cout << "SIGINT pending" << endl;
  }
  cout << "DONE\n";
  cout << "ESTABLISH OLD BLOCKED SIGNALS MIASK\n";
  old.set_as_process_signal_mask(SIG_SETMASK);
  reentrancy_writer writer(STDOUT_FILENO);
  writer.write("Bye\n");
  cout << "DONE\n";
  sleep(1);
  cout << "END\n";
  return 0;
}

static void sig_quit(int signo) {
  reentrancy_writer writer(STDOUT_FILENO);
  writer.write("caught SIGINT\n");
  if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
    writer.write("cannot reset sigquit");
    return; 
  }
  writer.write("DONE");
}
