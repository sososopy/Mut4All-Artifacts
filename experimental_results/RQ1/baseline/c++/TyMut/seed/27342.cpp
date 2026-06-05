
template<typename T> void f(T *p) { p->template ~X(); }
template<typename T> struct X {};
void g(X<int> *p) { f(p); }

