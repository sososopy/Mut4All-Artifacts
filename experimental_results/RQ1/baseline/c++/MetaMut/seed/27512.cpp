
  struct A {
    template<class>
    static constexpr const int &x=0;
  };
  template<class T=void>
  struct B {
    static constexpr int y=A::template x<int>;
  };
  template struct B<>;

