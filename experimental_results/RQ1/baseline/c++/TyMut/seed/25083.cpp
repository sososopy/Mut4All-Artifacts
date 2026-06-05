
constexpr _Complex int a {1, 1};

static_assert (& __imag a == &__real a, "");
