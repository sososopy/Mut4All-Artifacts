#include <concepts>
struct S { int i; };
static_assert(requires(S s) { requires std::destructible<decltype(s.i)>; });

