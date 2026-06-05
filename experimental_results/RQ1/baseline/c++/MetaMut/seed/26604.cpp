
struct A {
    char a;
    static constexpr int (*off_p)(int p) =
        [](int off = offsetof(A, a)) static constexpr ->int { return off; };
};

