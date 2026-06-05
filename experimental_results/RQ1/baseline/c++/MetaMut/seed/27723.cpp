
struct A { void (A::*p)(); };
template <A> struct X { };
X<A{ 0 }> x;

