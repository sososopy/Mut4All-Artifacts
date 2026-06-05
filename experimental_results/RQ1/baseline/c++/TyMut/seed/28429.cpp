
  template<auto const& ... Args>
  struct S {
    template<typename=void>
    void operator()() const {}
  };

  struct weird_ {
    int operator&() const { return 123; }
  } const weird {};

  auto f() {
    S<weird> s {};
    s();
  }

