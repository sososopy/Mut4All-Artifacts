
template<bool = __has_nothrow_assign(void)> struct A {};

A<> a;
