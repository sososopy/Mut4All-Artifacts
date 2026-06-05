

struct A { union { int x; }; };
struct B : private A { using A::x; } b;
int main (int, char**) { b.x = 0; return 0; }

