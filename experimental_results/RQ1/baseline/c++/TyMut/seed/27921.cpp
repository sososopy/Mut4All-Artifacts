
struct S { const char a[2]; };

constexpr struct S a[1] = { { "" } };

static_assert ('\0' == *a[0].a, "!");
