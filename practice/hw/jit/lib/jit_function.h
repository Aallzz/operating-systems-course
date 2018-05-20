#ifndef MY_JIT_FUNCTION 
#define MY_JIT_FUNCTION

#include <vector>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "operation_code.h"
#include "my_utility.h"
#include "memory_manager.h"

template<typename R>
struct jit_funtction { 

  std::vector<uint8_t> function_code;
  void *call;

  jit_funtction(const std::string& expr) {
    using namespace operation_code;

    std::vector<double> constants;
    auto tokens = split_string(expr);
    uint8_t cnt_reg{};
   
    for (auto const& token : tokens) {
      if (is_operation(token)) {
        uint8_t idx1 {std::min<uint8_t>(cnt_reg - 1, max_cnt_reg)};
        uint8_t idx2 {std::min<uint8_t>(cnt_reg, max_cnt_reg + 1)};
        if (cnt_reg < 2) {
          throw std::invalid_argument(std::string("Operation ") + token + std::string(" needs two operands")); 
        }
        if (cnt_reg >= max_cnt_reg) { 
          append(pop_xmm(max_cnt_reg));
        }
        if (cnt_reg > max_cnt_reg) { 
          append(pop_xmm(max_cnt_reg + 1));
        }
        append(op_xmm(get_xmm_operation(token[0]), idx1, idx2)); 
        if (cnt_reg > max_cnt_reg) { 
          append(push_xmm(max_cnt_reg));
        }
        cnt_reg--;
      } else if (token == "x") {
        if (cnt_reg + 1 >= max_cnt_reg) {
          append(push_xmm(0));
        } else {
          append(mov_xmm_idx1_from_idx2(cnt_reg + 1, 0));
        }
        cnt_reg++;
      } else if (is_double(token)) {
        constants.push_back(strtod(token.c_str(), NULL));
        append(load_xmm(std::min<uint8_t>(cnt_reg + 1, max_cnt_reg), 16 * (constants.size() - 1))); 
        if (cnt_reg + 1 >= max_cnt_reg) {
          append(push_xmm(max_cnt_reg));  
        }
        cnt_reg++;
      } else {
        throw std::invalid_argument(std::string("Unexpected token in expression: ") + token);
      }

    }
    append(mov_xmm_idx1_from_idx2(0, 1));
    append(ret);
  
    
    int32_t sz = function_code.size() + 7;
    sz = sz + 15 - (sz - 1) % 16;

    back_append(lea_rcx_rip_p(sz - 7));
    append(std::vector<uint8_t>(sz - function_code.size(), 0x00));
    
    for (double val : constants) {
      function_code.insert(function_code.end(), sizeof(double), 0x00);
      *reinterpret_cast<double*>(&function_code[function_code.size() - sizeof(double)]) = val;
      function_code.insert(function_code.end(), sizeof(double), 0x00);
      *reinterpret_cast<double*>(&function_code[function_code.size() - sizeof(double)]) = 0; 
    }
 
    using namespace memory_manager;
   /* 
    for (uint8_t const& x : function_code) {
       fprintf("%.2X ", x);
    }
    fprintf("\n");
     */
    call = map(16 * function_code.size(), -1, 0);
    memcpy(call, function_code.data(), function_code.size() * sizeof(uint8_t));
    protect(call, 16 * function_code.size(), PROT_READ | PROT_EXEC);
      
  }


  template<typename T>
  R operator()(T arg) const {
    return reinterpret_cast<R(*)(T)>(call)(arg);
  }

  ~jit_funtction() {
    using namespace memory_manager;
    protect(call, 16 * function_code.size(), PROT_READ | PROT_WRITE);
    unmap(call, 16 * function_code.size());
  }

private:
  
  const std::string allowed_operations = "+-*/";  
  const uint8_t max_cnt_reg = 6;

  bool is_operation(std::string const& str) const {
    return allowed_operations.find(str) != std::string::npos;  
  }

  bool is_double(std::string const& str) const {
    if (isdigit(str[0]) || (str.length() >= 2 && str[0] == '-' && isdigit(str[1]))) {
      char * end;
      double result = 0;
      errno = 0;
      result = strtod(str.c_str(), &end);
      return !(*end != '\0' || (result == 0 && (errno != 0 || end == str.c_str())));
    } else {
      return false;
    }
  }

  operation_code::xmm_operation get_xmm_operation(char const c) const {
        switch (c) {
          case '+': return operation_code::xmm_operation::plus;
          case '-': return operation_code::xmm_operation::minus;
          case '*': return operation_code::xmm_operation::mult;
          case '/': return operation_code::xmm_operation::div;
        }
        throw std::exception();
  }

  void append(std::vector<uint8_t>& dst, std::vector<uint8_t> const& src) {
    dst.insert(dst.end(), src.begin(), src.end());
  }

  
  void append(std::vector<uint8_t>& dst, std::vector<uint8_t>&& src) {
    dst.insert(dst.end(), std::make_move_iterator(src.begin()),
        std::make_move_iterator(src.end()));
  }
  
  template<typename T>
  void append(T&& src) {
    append(function_code, std::forward<T&&>(src));
  }

  void back_append(std::vector<uint8_t>& dst, std::vector<uint8_t> const& src) {
    dst.insert(dst.begin(), src.begin(), src.end());
  }
  
  void back_append(std::vector<uint8_t>& dst, std::vector<uint8_t>&& src) {
    dst.insert(dst.begin(), std::make_move_iterator(src.begin()), 
        std::make_move_iterator(src.end()));
  }

  template<typename T>
  void back_append(T&& src) {
    back_append(function_code, std::forward<T&&>(src));
  }

};

#endif

