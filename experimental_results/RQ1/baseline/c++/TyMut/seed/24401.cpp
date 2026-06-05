
struct A {
  void foo(const int &);
  void foo(float);
};

template <typename... Args>
void bar(void (A::*memfun)(Args...), Args... args);

void go(const int& i) {
  bar<const int &>(&A::foo, i);
}


