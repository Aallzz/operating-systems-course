#ifndef MY_REENTRANCY_WRITER
#define MY_REENTRANCY_WRITER

#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <type_traits>

namespace reentrancy_io {

enum class numeric_base {
  hex_little_endian,
  hex_big_endian,
  dec,
  //oct,
  //bin
};

struct reentrancy_writer {

  const static char mp [16];

  reentrancy_writer() = delete;
  reentrancy_writer(reentrancy_writer const&) = delete;
  reentrancy_writer& operator =(reentrancy_writer const&) = delete;

  reentrancy_writer(int const fd); 

  reentrancy_writer(std::string_view const& file_name);

  reentrancy_writer const& write(std::string_view const& text) const;
    
  template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
  reentrancy_writer const& write(T number, numeric_base b, bool prefix = true) const {
    char buffer[100];
    if (b == numeric_base::dec) {
      int pos {};
      if (number == 0) {
        pos = 1;
        buffer[0] = 0;
      }
      while (number) {
        buffer[pos++] = number % 10;
        number /= 10;
      }
      for (int i = 0; i < pos / 2; ++i) {
        std::swap(buffer[i], buffer[pos - 1 - i]); 
      }
      buffer[pos] = '\0';
      return write(buffer);
    } else if (b == numeric_base::hex_big_endian || b == numeric_base::hex_little_endian) {
      int pos {};
      T msk {0xF};
      int add {prefix ? 2 : 0};
      while (pos < sizeof(T) * 2) {
        buffer[add + pos++] = mp[(number & msk)];
        number >>= 4ll;
      }
      if (prefix) {
        buffer[0] = '0';
        buffer[1] = 'x';
      }
      if (b == numeric_base::hex_big_endian) {
        for (int i = 0; i < pos / 2; ++i) {
          std::swap(buffer[add + i], buffer[add + pos - 1 - i]);  
        }
      }
      buffer[pos + add] = '\0';
      return write(buffer);
    }
  }
  
  ~reentrancy_writer(); 

private:

  bool owner {false};
  int fd;

};

}
#endif
