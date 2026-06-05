

void f() {}

static constexpr void (*g1)() = &f; // ok
static constexpr void (*g2)() = f; // ok
struct S {
    static constexpr void (*g3)() = &f; // ok
    static constexpr void (*g4)() = f; // error: field initializer is not constant
};
