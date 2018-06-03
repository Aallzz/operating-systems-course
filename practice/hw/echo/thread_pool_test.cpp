#include <bits/stdc++.h>
#include "lib/thread_pool.h"
#include "lib/blocking_queue.h"
#include <random>

using namespace std;
using namespace pied_piper;

//#define test1

int main() {

  int n = 1000000;
  
#ifndef test1
  thread_pool executor(11);
  vector<future<int>> res(n);
  for (int i = 0; i < n; ++i) {
    res[i] = std::move(executor.submit([](int x){return x*x;}, i));
  }
  cout << 1 << endl;
  for (int i = 0; i < n; ++i) {
    res[i].get(); 
  }
  cout << 2 << endl;
#else
  vector<int> res(n);
  for (int i = 0; i < n; ++i) {
    res[i] = i * i;
  }
#endif
  return 0;
}
