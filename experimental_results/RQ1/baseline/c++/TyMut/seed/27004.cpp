

template <typename T>
constexpr bool Val = true;

template <class I>
concept bool InputIterator() {
  return requires (I i) {
    requires Val <decltype(i++)>;
  };
}

template <class I>
concept bool ForwardIterator() {
  return InputIterator<I>() && true;
}

template<InputIterator>
constexpr bool f() { return false; }
template<ForwardIterator>
constexpr bool f() { return true; }

static_assert(f<int*>());
