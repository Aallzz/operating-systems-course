#ifndef MY_RUN_TIME_LIB_FUNCTION
#define MY_RUN_TIME_LIB_FUNCTION

#include <dlfcn.h>
#include <memory>
#include <type_traits>
#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>

template<typename T>
struct run_time_lib_function;

template<typename R, typename... Args>
struct run_time_lib_function<R(Args...)> {

  run_time_lib_function(std::string const& path) {
    if ((lib_handle = dlopen(path.c_str(), RTLD_LAZY)) == NULL) {
      throw std::runtime_error(dlerror()); 
    }
  }

  void load_function(std::string const& function_name) {
    if (!(func = reinterpret_cast<R(*)(Args...)>(dlsym(lib_handle, function_name.c_str())))) {
      throw std::runtime_error(dlerror());
    }
  }

  R operator()(Args... args) const {
    return func(args...);  
  }

  ~run_time_lib_function() {
    dlclose(lib_handle);
  }

private:
  std::function<R(Args...)> func;
  void* lib_handle;
};

#endif
