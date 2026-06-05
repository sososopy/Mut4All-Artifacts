Short example:

struct B {
    int i;
    CONSTEXPR bool operator==(B const&) const = default;
};

struct D : B {
    CONSTEXPR bool operator==(D const&) const = default;
};

static_assert(D{0} == D{5});

When compiled with -DCONSTEXPR=constexpr, the static assertion passes. When compiled with -DCONSTEXPR=, the static assertion fails.

For some reason, when the equality operator is declared constexpr, there is just no work that happens for doing equality. For instance, the function

bool check(D a, D b) {
    return a == b;
}

compiles to:

check(D, D):
        mov     eax, 1
        ret
---------------------------------------------------------

