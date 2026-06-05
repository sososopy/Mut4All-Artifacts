

template <class T> concept foo =
  requires(T& x, void(fun)(int &)) { fun(x.a); };

struct A {
  int a, b;
};
struct B {
  int a : 4;
  int b : 4;
};

bool a = foo<A>;
bool b = foo<B>;
