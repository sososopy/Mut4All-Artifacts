
struct A {
   const A &a; int x = 2;
};

extern A a0;

#if DESIGNATED
# define DESIGNATOR .a =
#else
# define DESIGNATOR
#endif
constexpr int x = A{ DESIGNATOR A{ a0, 4 } }.x;

static_assert(x != 2, "Expected diagnostic");

int main() { }


