

#include <iostream>

template <typename T, size_t N>
constexpr size_t array_size(const T (&)[N])
{
  return N;
}

class X {
public:
  struct item {
    char const* const name;
    int               price;
  };

  static const     item arr0[2];
  static constexpr item arr1[3] = {{"asdf", 0}, {"qwer", 1}, {"zxcv", 2}};

  static size_t constexpr size0() {
    return array_size(arr0);    // perfect
  }

  static size_t constexpr size1() {
    return array_size(arr1);    // problem!!
  }
  
};

const X::item X::arr0[] = {{"asdf", 0}, {"qwer", 1}};

int main()
{
  X x;
  std::cout << x.size0() << std::endl; // perfect
  std::cout << x.size1() << std::endl; // ERROR IN THIS LINE (cannot link!??)
  return 0;
}
