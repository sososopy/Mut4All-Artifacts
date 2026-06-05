
int f();
static_assert(noexcept(sizeof(f())), "");  // Fails on GCC
