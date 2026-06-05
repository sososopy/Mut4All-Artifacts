
struct S { operator long & (); } b;
template <int N> void foo () { b--; }
void bar () { foo <0> (); }
