
struct integer {};

integer f();

int operator+(integer, integer);

using max_type = decltype(f() + f());
