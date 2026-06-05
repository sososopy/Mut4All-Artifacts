
#include <stl2/iterator.hpp>

template <stl2::WeakOutputIterator<int> I>
  requires !stl2::WeakInputIterator<I>()
constexpr bool dispatch() { return false; }

template <stl2::WeakInputIterator I>
constexpr bool dispatch() { return true; }

template <stl2::WeakOutputIterator<int> I>
constexpr bool is_weak_out() { return true; }

template <stl2::WeakInputIterator I>
constexpr bool is_weak_in() { return true; }

int main() {
  static_assert(is_weak_out<int*>());
  static_assert(is_weak_in<int*>());
  static_assert(dispatch<int*>());
}

