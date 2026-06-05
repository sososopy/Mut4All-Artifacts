
template <typename T> void foo() noexcept {}
template <typename T> void bar() throw() {}
template void foo<int>();
template void bar<int>();

