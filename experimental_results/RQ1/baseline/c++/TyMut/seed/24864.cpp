int one = 1;
auto factory(int& x = one) {
    return [&](this auto self) {
      x;
    };
};

using Base = decltype(factory());
struct Derived : Base {
  Derived() : Base(factory()) {}
};

int main() {
  Derived d;
  d();
}