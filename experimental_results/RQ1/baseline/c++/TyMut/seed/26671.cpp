

template <bool...> struct bool_sequence {};

template <class T, class...Us>
concept bool Same =
  __is_same_as(bool_sequence<__is_same_as(T, Us)...>,
               bool_sequence<(true || __is_same_as(T, Us))...>);

template <class I>
concept bool A =
  requires (I& i) {
    requires Same<I&, decltype(++i)>;
  };

template <class I>
concept bool B = A<I> && true;

template <A>
constexpr bool f() { return false; }
template <B>
constexpr bool f() { return true; }

int main() {
  static_assert(f<int>());
}

