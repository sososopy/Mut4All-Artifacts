
template <int x, int y = x+1>
class A {};

void foo(A<0> &M) {
  A<0, 0> N = M;
}

