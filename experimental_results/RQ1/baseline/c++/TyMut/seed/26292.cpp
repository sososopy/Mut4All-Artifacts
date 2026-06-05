
template<typename T> struct A { friend void f(A<T>) {} }; void g() { f(A<int>()); }


