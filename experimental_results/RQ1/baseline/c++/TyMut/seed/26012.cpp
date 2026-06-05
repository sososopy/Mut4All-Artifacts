
  template <typename T = Foo> class C;
  template <typename T> class C {
    int x;
  };
  typedef int Foo;
  C<> c;

