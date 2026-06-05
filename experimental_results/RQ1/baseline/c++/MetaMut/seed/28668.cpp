
struct A { int v; };
template <typename T> struct B : T {};
struct C : B<A> {};
int main() { return C{}.B::B<A>::v; }

