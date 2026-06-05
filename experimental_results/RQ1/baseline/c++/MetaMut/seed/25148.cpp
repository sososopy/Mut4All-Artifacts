
template<int n>
void Foo();

template<int n>
struct BarImpl {
  static void Apply(int m) {
    if (m == n) {
      Foo<n>();
    } else {
      BarImpl<n-1>::Apply(m);
    }
  }
};

template<>
struct BarImpl<0> {
  static void Apply(int m) {
    Foo<0>();
  }
};

void Bar(int n) {
  BarImpl<30>::Apply(n);
}
