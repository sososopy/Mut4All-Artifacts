
constexpr int *p = 0;
constexpr int &r = *p;
static_assert (&r != 0, "");
