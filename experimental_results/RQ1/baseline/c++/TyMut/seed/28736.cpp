
template <int> struct A {};
template <A a> struct S { void foo(S<+a>); };

