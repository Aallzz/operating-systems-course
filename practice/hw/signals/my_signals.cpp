#include <bits/stdc++.h>
#include "lib/reentrancy_writer.h"
#include "lib/my_signal.h"

using namespace reentrancy_io;
using namespace signal_helper;

static void dumper(int signo, siginfo_t* siginfo, void* cntxt);

int main(int argc, char* argv[]) {

  if (argc == 1) {
    std::cout << "Usage: ./my_signals <raise | str | null>" << std::endl;
    return 0;
  }
  
  signals::factory()
    .add_flag(SA_NODEFER)
    .set_handler(dumper)
    .set_for_signal(SIGSEGV);
  
  if (strcmp(argv[1], "str") == 0) {
      strcat("a", "b");
  } else if (strcmp(argv[1], "null") == 0) {
    int *p = nullptr;
    *p = 10;
  } else if (strcmp(argv[1], "raise") == 0) {
    raise(SIGSEGV);
  }
  
  return 0;
}

static void check_access (int signo) {
  signals::longjmp(1);
}

static void dumper (int signo, siginfo_t* siginfo, void* cntxt) {
  ucontext_t * context = reinterpret_cast<ucontext_t*>(cntxt);
  reentrancy_writer writer(STDIN_FILENO);
  
  writer.write("------------------------------------Memory image------------------------------------\n");    
  
  writer.write(" SIGSEGV on address  ")
    .write(reinterpret_cast<uint64_t>(siginfo->si_addr), numeric_base::hex_big_endian)
    .write("\n")
    .write("Shadow bytes around buggy address:\n");
  
  signals::factory()
    .add_flag(SA_NODEFER)
    .set_handler(check_access)
    .set_for_signal(SIGSEGV);
  
  uint64_t page = reinterpret_cast<uint64_t>(siginfo->si_addr) & ~0xF;
  for (int64_t page_shift = -5 * 16; page_shift <= 5 * 16; page_shift += 16) {
    if ((int64_t)page < -page_shift) continue;
    writer.write(page_shift == 0 ? "=>" : "  ")
      .write(page + page_shift, numeric_base::hex_big_endian)
      .write(": ");
    for (uint8_t shift = 0; shift < 16; ++shift) {
        if (signals::setjmp(1)) {
          writer.write("00  ");  
        } else {
          uint8_t temp = *reinterpret_cast<uint8_t*>(page + page_shift + shift);
          writer.write(temp, numeric_base::hex_big_endian, false).write("  ");
        }
    }
    writer.write("\n");
  }
 
  // https://code.woboq.org/userspace/glibc/sysdeps/unix/sysv/linux/x86/sys/ucontext.h.html
  
  char const reg_names[23][10] = {
    "R8\0",  "R9\0",  "R10\0",    "R11\0", "R12\0",     "R13\0",      "R14\0", "R15\0",
    "RDI\0", "RSI\0", "RBP\0",    "RBX\0", "RDX\0",     "RAX\0",      "RCX\0", "RSP\0",
    "RIP\0", "EFL\0", "CSGSFS\0", "ERR\0", "TRAPNO\0",  "OLDMASK\0",  "CR2\0"
  };

  writer.write("Registers: \n");
  for (int i = 0; i < 23; ++i) {
    writer.write("").write(reg_names[i]).write("\t:\t")
      .write(context->uc_mcontext.gregs[i], numeric_base::hex_big_endian)
      .write(i % 2 == 0 ? "\t" : "\n");
  }
  writer.write("\n");
  exit(errno);
};

