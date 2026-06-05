struct S { bool operator==(const S&) const = default; int s; };
static_assert(S{} == S{});

