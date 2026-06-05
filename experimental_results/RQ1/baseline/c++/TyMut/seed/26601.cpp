
template<bool B>
struct S {};
template<int... C>
using u = S<true>;
struct X {};
X a = u {};

