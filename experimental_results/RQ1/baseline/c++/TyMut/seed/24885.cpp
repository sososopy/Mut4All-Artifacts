
struct Base {
  operator int(this auto&&) {
    return 42;
  }
};

struct Derived : Base {};

int main() {
  Derived derived;
  return static_cast<int>(derived);
}

