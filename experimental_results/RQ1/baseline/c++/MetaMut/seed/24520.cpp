
struct foo {
  template <int size>
  struct bar {
    char* p = new char[size];
  };
};

foo::bar<10> moo;

