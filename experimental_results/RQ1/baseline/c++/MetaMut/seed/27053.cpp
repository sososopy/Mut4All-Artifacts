

#include <stl2/detail/swap.hpp>

struct I {
  int operator*() const;
};

template <class R>
using Ref = decltype(*std::declval<R>());

template <class U = I>
  requires !__stl2::Swappable<Ref<I>, Ref<I>>()
static constexpr bool bar() { return true; }

template <class U = I>
  requires !__stl2::Swappable<Ref<U>, Ref<U>>()
static constexpr bool foo() { return true; }

static_assert(bar()); // Fine
static_assert(foo()); // Error

