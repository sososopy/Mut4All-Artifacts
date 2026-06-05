
template <typename T>
using Vec __attribute__((vector_size(16))) = T;

template <typename T>
void foo(Vec<T> arg1, T arg2) {}

void bar() {
    foo(Vec<int>{}, 1);
}
