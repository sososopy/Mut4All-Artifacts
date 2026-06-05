
struct S;
template <class T> void foo () noexcept (T::v);
template <class ...A>
void bar (void (A...) noexcept)
{ bar (foo<S>); }

