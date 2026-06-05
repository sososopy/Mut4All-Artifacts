
template <typename> struct Foo {
  ~Foo()
    requires is_scalarvalue;
  ~Foo()
    requires is_unionvalue;
};

Foo<int> foo3 {};
