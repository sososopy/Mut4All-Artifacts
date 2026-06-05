
template<typename T, int N>
struct A { T t[N]; };

template<typename T>
struct B { T t[2]; };

int main()
{
   A a{{1, 2}}; // ok in gcc
   B b{{1, 2}}; // error in gcc (ok in other compilers, e.g. msvc)
   return a.t[0];
}
