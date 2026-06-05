
template <typename T> struct A { alignas(T) alignas(int) int a; };
struct B { B(const char *, const char *, int, int); A<int> b; };
B c {"", "", 0, 0};
