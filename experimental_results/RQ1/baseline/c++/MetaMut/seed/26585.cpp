
template<class T> auto f(T t) -> decltype(++t) { return t; }
void f(...) {}
void g() { f(true); }
