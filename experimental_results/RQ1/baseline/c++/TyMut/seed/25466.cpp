

#include <decimal/decimal>
double get_double();
std::decimal::decimal64 test() {
  return get_double();  // invalid conversion
}

