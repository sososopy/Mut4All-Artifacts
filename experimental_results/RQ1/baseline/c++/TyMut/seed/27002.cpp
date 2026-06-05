

$ cat rdseed.cxx
#include <iostream>
#include <immintrin.h>

int main()
{
  unsigned long long val = 0;
  _rdseed64_step(&val);
  _rdseed32_step(&val);
  _rdseed16_step(&val);

  std::cout << val << std::endl;

  return !val;
}

