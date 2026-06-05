
#include <compare>

struct B {
  B(int i) : i(i) {}

  VIRTUAL std::strong_ordering operator<=>(B const& other) const = default;
  int i;
};

struct D : B {
  D(int i, int j) : B(i), j(j) {}
  int j;
};

bool check() {
    return B(2) == D(2, 3);
}

