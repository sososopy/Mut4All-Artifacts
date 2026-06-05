

  template<typename T> struct S { static constexpr int a[] {0}; };

  template struct S<void>;

  int main() {}
