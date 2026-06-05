
struct S {
    int i;
    int *p = &i;
    constexpr const int* ptr() const { return &i; }
};

// Not constant expression in GCC.
static_assert(S{2}.ptr() != nullptr);
static_assert(S{2}.p != nullptr);

// Is a constant expression.
static_assert(*S{2}.ptr() == 2);
static_assert(*S{2}.p == 2);

static_assert([]() constexpr {
    S r(2);

    // Is a constant expression.
    return r.ptr() != nullptr && r.p != nullptr;
}());
