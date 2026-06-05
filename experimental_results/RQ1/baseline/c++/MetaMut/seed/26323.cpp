
template<typename T> int foo(T) noexcept(T()) = delete;

int i = foo(0);
