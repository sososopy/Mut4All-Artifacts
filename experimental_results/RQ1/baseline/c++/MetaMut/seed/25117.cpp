

struct A { union { long s; char d[1]; }; constexpr A (char x) : s(0) { d[0] = x; } };
struct B { A b[2]; };
B c { 'a', 'b' };
