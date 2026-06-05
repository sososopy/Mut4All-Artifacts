struct A {};

template<class T, class U>
void f(A<T, U>&) {}

template<class T>
struct B : A<T, long> {};

template<class T>
struct C : A<T, char> {};

template<class T>
struct D : B<T>, C<T> {};

int main()
{
  D<int> d;
  f(d);
}
