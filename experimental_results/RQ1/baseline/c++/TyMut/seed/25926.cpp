
struct X { X(); };

template<typename T> struct Test {
  union {
    struct {
      X x;
    };
  };
}

