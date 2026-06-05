
inline int foo()
{
  static int static_local;
  return ++static_local;
}
+ cat lib.cc
#include "foo.h"

extern "C" void lib();

void lib()
{
  static_cast<void>(foo());
}
+ cat main.cc
#include <cstdlib>

#include <iostream>

#include <dlfcn.h>

#include "foo.h"

int main()
{
  void *lib_handle = dlopen("./lib.a", RTLD_NOW);

  if (!lib_handle) {
    std::cout << "dlopen() failed: " << dlerror() << '\n';

    std::exit(1);
  }

  void *func = dlsym(lib_handle, "lib");

  if (func) {
    reinterpret_cast<void (*)()>(func)();

  } else {
    const char *err = dlerror();

    if (!err) {
      std::cout << "lib() not found\n";
    } else {
      std::cout << "dlsym() fail: " << err << '\n';
    }
  }

  // Should output 2.
  //
  std::cout << foo() << '\n';

  dlclose(lib_handle);

  return 0;
}
