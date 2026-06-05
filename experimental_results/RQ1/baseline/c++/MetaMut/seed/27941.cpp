void foo1() noexcept(true) {}
decltype(foo1) stub_foo1;
void stub_foo1() noexcept(true) {}

