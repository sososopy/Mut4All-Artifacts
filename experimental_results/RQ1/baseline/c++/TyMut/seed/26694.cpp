
template<typename T>
auto foo(T x) -> decltype(~x)
{ return ~x; }

int bar()
{ return foo(10); }

