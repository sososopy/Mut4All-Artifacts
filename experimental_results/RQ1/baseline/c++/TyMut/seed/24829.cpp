
#include <iostream>

#define barf(msg) [[unlikely]] crash(msg)

void
crash (const char*msg)
{
  std::cerr << msg << std::endl;
  abort ();
}


void
if_dual_warning (int i)
{
  bool runtime_cond0 = i > 0;
  bool runtime_cond1 = i > 1;

  if (runtime_cond0) {
    std::cerr << "Something\n";
  } else if (runtime_cond1) {
    barf("foo");
  } else {
    barf("bar");
  }
}
