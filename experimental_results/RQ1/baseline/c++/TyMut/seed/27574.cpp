
struct A { int x; };
extern A a;
template<int...> struct B {};
template<int y>
struct B<y, sizeof(a.x)> {};


