template<typename Type> struct Test { constexpr Test() {} };
template<typename T> void test() { Test<T> x; } 
void f() { test<int>(); }
