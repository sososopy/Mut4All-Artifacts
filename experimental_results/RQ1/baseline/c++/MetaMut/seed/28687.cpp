
struct sso {
    union {
        int buf[10];
        int* alloc;
    };
};

constexpr bool test_switch() noexcept {
    sso val;
    val.alloc = nullptr;
    val.buf[5] = 42;
    return true;
}
static_assert(test_switch());

constexpr void perform_assignment(int& left, int right) noexcept {
    left = right;
}

constexpr bool test_switch_with_indirection() noexcept {
    sso val;
    val.alloc = nullptr;
    perform_assignment(val.buf[5], 42);
    return true;
}
static_assert(test_switch_with_indirection());


