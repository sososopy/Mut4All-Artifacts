

  #include <cassert>

  struct A
  {
    int x, y;
  };

  template <typename T>
  int f1(T t1, T t2)
  {
    A a[] = { t1, t2 };
    return sizeof(a) / sizeof(a[0]);
  }

  void test1()
  {
    assert(f1(A(), A()) == 2); // OK
    assert(f1(1, 2) == 1); // ERROR: this assert fails at runtime
  }


  template <typename ...T>
  int f2(T ...n)
  {
    A a[] = { n... };
    return sizeof(a) / sizeof(a[0]);
  }

  void test2()
  {
    assert(f2(A(), A()) == 2); // OK
    assert(f2(1, 2) == 1); // OK
  }

