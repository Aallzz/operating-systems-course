#include <bits/stdc++.h>
#include <dlfcn.h>

#include "lib/process_info.h"
#include "lib/square_pattern.h"
#include "lib/run_time_lib_function.h"

using namespace std;

using print_cirlce_type = void(FILE*, int);

int get_int(string const& arg) {
  int x = 10;
  try {
    x = stoi(arg);
  } catch (std::exception const& e) {
    cout << "Incorrect third argument: " << e.what() << endl;
  }
  return x;
}

int main(int argc, char* argv[]) {
  
  process::process_info p;
  
  cout << "Initial memory info: " << endl;
  cout << "    " << p.get_info(process::part_info::data_size) << endl;
  cout << "    " << p.get_info(process::part_info::text_size) << endl;

  if (argc >= 2 && strcmp(argv[1], "circle") == 0) {
    int x = (argc >= 3 ? get_int(argv[2]) : 10);
    try {
      run_time_lib_function<print_cirlce_type> f("lib/libcircle_pattern.so");
      f.load_function("print_circle");
      p.update();
      cout << "Memory print after load run-time-loaded-function: " << endl;
      cout << "    " << p.get_info(process::part_info::data_size) << endl;
      cout << "    " << p.get_info(process::part_info::text_size) << endl;
      for (int i = 0; i < x; ++i) {
          f(stdout, i);
      }
      p.update();
      cout << "Memory print after call run-time-loaded-function: " << endl;
      cout << "    " << p.get_info(process::part_info::data_size) << endl;
      cout << "    " << p.get_info(process::part_info::text_size) << endl;
    } catch (std::runtime_error const& e) {
      cout << e.what() << endl;
    }
    p.update();
    cout << "Memory print after usage run-time-loaded-function: " << endl;
    cout << "    " << p.get_info(process::part_info::data_size) << endl;
    cout << "    " << p.get_info(process::part_info::text_size) << endl;
  } else {
    int x = (argc >= 3 ? get_int(argv[2]) : 10);
    while (x-- > 0) {
      for (int i = 4; i < 10; i += 2) {
        print_square(cout, i);   
      }
      for (int i = 10; i>= 4; i -= 2) {
        print_square(cout, i);
      }
    }
    clear_square(cout);
  }
  p.update();
  cout << "Memory print usage: " << endl;
  cout << "    " << p.get_info(process::part_info::data_size) << endl;
  cout << "    " << p.get_info(process::part_info::text_size) << endl;
  return 0;
}
