
  export module main;

  template <int N>
  union U {
  private:
    char a[N + 1];
    int b;
  };

  U<4> p;

