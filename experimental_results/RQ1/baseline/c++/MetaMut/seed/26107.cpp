

struct A {
  ~A() __attribute__((noreturn));
  A& stream();
  template <typename T> void operator<<(T&);
};

struct C {
  enum State { S1, S2 };
  static bool Fn(State s) {
    switch (s) {
      case S2: A().stream() << s;
    }
  }
};

void TestFn()
{
  for (int i = 0; i < 2; ++i) {
    C::Fn ? (void) 0 : A().stream() << " ";
    C::Fn(static_cast<C::State>(i));
  }
}


