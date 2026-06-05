
extern "C" void abort();
void bar();

namespace {
  struct A { A(int); char sizer[1]; };
}

template <typename T> void foo(char (*)[sizeof A(T(0))]) {
  static unsigned cnt = 0;
  if (cnt++) abort();
}

int main(void) { bar(); foo<int>(0); }


