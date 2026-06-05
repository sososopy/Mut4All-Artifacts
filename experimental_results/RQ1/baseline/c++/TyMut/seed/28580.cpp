
template <class T> concept C = requires (T a) { a.f(0); };
struct S { void f(auto) noexcept(); };
static_assert(C<S>);


