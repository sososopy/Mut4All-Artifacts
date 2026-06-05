
#include <cstdlib>

static int debug_level()
{
  return !!getenv("DEBUG");
};

template <typename T>
struct foo
{
  foo() : debug_(debug_level()) {}
  int debug_;
};
