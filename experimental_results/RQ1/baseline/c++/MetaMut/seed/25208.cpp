

struct A { void operator()(); };
struct B { void operator()(); };
struct C : A, B {};

template<class T>
decltype(T()()) foo(int);

template<class> int foo(...);

using type = decltype(foo<C>(0));
using type = int;

