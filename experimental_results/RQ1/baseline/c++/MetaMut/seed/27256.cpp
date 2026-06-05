
  struct F {F(F&&)=delete;};

  template<int=0>
  struct M {
    F f;
    M();
    M(const M&);
    M(M&&);
  };

  template<int I>
  M<I>::M(M&&)=default;

  M<> f() {
    M<> m;
    return m;
  }

