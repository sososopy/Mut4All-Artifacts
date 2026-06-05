

template <class T, class A>
void f() {
  T *q;
  q->A::~A();
}

class Q {};
int main() {
  f<Q, int>();
}

