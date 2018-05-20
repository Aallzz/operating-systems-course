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
  jit_funtction<double> fun(expr);
  cout << std::setprecision(2) << std::fixed << fun(x) << endl;
  return 0;
}
