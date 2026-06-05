
#include <cstdint>
#include <memory>

#define visibility_hidden __attribute__((visibility("hidden")))
#define visibility_default __attribute__((visibility("default")))

struct visibility_default base_spec_char
  {
  using value_type  = char;
  char foo[10];
  };
  
//SPECIALIZING ON THIS TYPE WILL CAUSE MISSING CODE AND SYMBOLS
struct visibility_hidden base_spec_uint8
  {
  using value_type  = uint8_t;
  uint8_t foo[20];
  };
  
template<typename VarStoreType>
struct visibility_default var_t
  {
  using var_store_t = VarStoreType;
  using value_type  = typename var_store_t::value_type;

  std::unique_ptr<var_store_t> alloc();
  };
using varchar_t = var_t<base_spec_char>;
using varbinary_t = var_t<base_spec_uint8>;


#include "some_shared.h"

template<typename T>
std::unique_ptr<typename var_t<T>::var_store_t>
 var_t<T>::alloc( )
  {
  return std::make_unique<var_store_t>();
  }
  
template struct var_t<base_spec_char>;
template struct var_t<base_spec_uint8>;

//MAIN PROGRAM LINKED TO SHARED LIBRARY
#include <iostream>
#include "some_shared.h"

int main(int argc, char **argv) 
  {
  varchar_t foo;
  varbinary_t bar;
  
  auto obj = foo.alloc();
  auto obj2 = bar.alloc(); //<- unresolved symbols
  std::cout << "Hello, world!"<< obj.get()<< obj2.get() << std::endl;
  return 0;
  }
