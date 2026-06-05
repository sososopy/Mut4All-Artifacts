template<class T>
struct A {};

void f(A<long>&) {}
void f(A<char>&) {}

template<class T>
void g(A<T>&) {}

struct B : A<long> {};

struct C : A<char> {};

struct D : B, C {};

int main()
{
  D d;
  // f(d);  // ambiguous
  g(d);
}

