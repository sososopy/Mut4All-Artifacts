
template<typename T> struct S { int a; T x; };
template<typename T> decltype(S<T>().~S()) f();
void g() { f(); }
