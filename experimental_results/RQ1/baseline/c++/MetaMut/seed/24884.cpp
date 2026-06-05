template <auto T> struct A {};
template <auto T> A<*T> operator *() { return {}; }