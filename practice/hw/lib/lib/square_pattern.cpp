#include "square_pattern.h"

std::ostream& print_square(std::ostream& out, int x) {
  assert(x < 20);  
  assert(x >= 2);
  using std::endl;

  auto ub = std::string((20 - x) / 2 + (20 - x) % 2, ' ') + "+" +
    std::string(x - 2, '-') + "+" + std::string((20 - x) / 2, ' ');
  auto md = std::string((20 - x) / 2 + (20 - x) % 2, ' ') + "|" +
    std::string(x - 2, ' ') + "|" + std::string((20 - x) / 2, ' ');

  for (int i = 0; i < (20 - x) / 2 + (20 - x) % 2; ++i) 
    out << std::string(20, ' ') << endl;
  out << ub << endl;
  for (int i = 0; i < x - 2; ++i) {
    out << md << endl;    
  }
  out << ub << endl;
  for (int i = 0; i < (20 - x) / 2; ++i) 
    out << std::string(20, ' ') << endl;
  
  for (int i = 0; i < 20; ++i)
    out << "\033[F";
  usleep(30000);
  
  return out;
};

std::ostream& clear_square(std::ostream& out) {
  for (int i = 0; i < 20; ++i) {
    out << std::string(20, ' ') << std::endl;
  }
  for (int i = 0; i < 20; ++i) {
    out << "\033[F";
  }
  return out;
};
