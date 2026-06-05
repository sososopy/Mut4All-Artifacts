
template<typename ...T> struct C : T... {
  using typename T::type ...;
  void f() { type value; }
};

