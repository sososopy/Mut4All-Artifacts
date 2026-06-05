
#include <initializer_list>
struct D {
  D(std::initializer_list<double>);
};

struct E {
  explicit E(double, bool);
};

struct F {
  F(const E&);
  F(const D&);
};

void use() {
  F f({0., 1.});
}

