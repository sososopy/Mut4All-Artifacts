#include <any>
#include <vector>

struct OptionTestEntry
{
  int test1;
  std::vector<std::any> invalid_vals;
};

int main() {
  const OptionTestEntry testm[] = { {1, {-1}} };
  if (testm[0].invalid_vals.size() != 1)
    __builtin_abort();
};

