
#include <type_traits>

int main()
{
  static_assert(std::is_convertible<double, float>::value);   // ok
  static_assert(std::is_convertible<float, _Float16>::value); // fails
}
