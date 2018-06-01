#include <bits/stdc++.h>
#include "lib/my_signal.h"
#include "lib/reentrancy_writer.h"

using namespace std;
using namespace reentrancy_io;
using namespace signal_helper;

static void sig_quit(int);

int main() {

  try {
    signals::factory()
      .add_set(SIGINT)
      .set_handler(sig_quit)
      .set_for_signal(SIGINT);

    signals old_proc = signals::factory()
      .add_set(SIGINT)
      .set_as_process_signal_mask(SIG_BLOCK);
  
    sleep(1);
  
    if ( signals::get_pending_signals().is_member_set(SIGINT)) {
      cout << "SIGINT pending" << endl;
    }
  
    signals::factory().add_set(SIGINT).suspend();
    old_proc.set_as_process_signal_mask(SIG_SETMASK);

  } catch (...) {
    cout << ":(" << endl;
  }
  return 0;
}

static void sig_quit(int signo) {
  reentrancy_writer writer(STDOUT_FILENO);
  writer.write("caught SIGINT\n");
  try {
    signals::factory().set_handler(SIG_DFL).set_for_signal(SIGINT);
  } catch (...) {
    writer.write("cannot reset sigquit");
    return; 
  }
  writer.write("DONE");
}
