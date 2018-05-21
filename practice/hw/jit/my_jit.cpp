#include <bits/stdc++.h>
#include "lib/jit_function.h"

using namespace std;

int main(int argc, char* argv[]) {
  using std::string;
  using std::endl;
  using std::cout;
  if (argc != 3) {
    cout << argc << endl;
    cout << "Usage: ./my_jit filename <x float value>\n";
    return 0;
  }
  ifstream fin(argv[1]);
  double x = strtod(argv[2], NULL);
  string expr;
  getline(fin, expr);
  // expr = "3 x +";
  try {
    jit_funtction<double> fun(expr);
    cout << std::setprecision(2) << std::fixed << fun(x) << endl;
   // fun.change(2, '*');
   // cout << std::setprecision(2) << std::fixed << fun(x) << endl;
  } catch (std::invalid_argument& e) {
    cout << e.what() << endl; 
  }
  return 0;
}
