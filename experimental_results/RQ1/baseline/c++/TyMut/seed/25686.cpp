

template <int *p> class T {};

int a, b;
typedef T<&a> Ta;
typedef T<&b> Tb;

void f() {
  Tb tb;
  Ta ta(tb);
}
