
  template<int> struct A {};
  template<typename T> struct B : A<0> {
    B() : A<(0)>{} {}
  };

