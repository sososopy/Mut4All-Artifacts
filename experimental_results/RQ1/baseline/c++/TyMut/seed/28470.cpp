
struct S { int i; };
union U { S k; };
template <class T, class... Ts> T sink(T&&, Ts&&...);
template <class T>
decltype(sink(U{1},T())) f(T) { return U{1}; }
int main() { f(3); }


