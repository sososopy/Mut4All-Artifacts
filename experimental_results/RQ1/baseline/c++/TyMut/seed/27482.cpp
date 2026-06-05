
struct S;
template <class T> void foo () noexcept (T::v);
template <class ...A, int b>
void bar (void (A...) noexcept (b));
static_assert (bar (foo<S>);

