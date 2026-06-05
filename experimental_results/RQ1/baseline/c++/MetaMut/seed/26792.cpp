struct X {const int val;};
static_assert(__is_trivially_copyable(const int), "");
static_assert(__is_trivially_copyable(X), "");

