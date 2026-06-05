

template <class T, class U>
concept bool Same = __is_same_as(T, U);

template <class T>
concept bool C =
  requires (T t) {
    requires Same<decltype(t),void>;
  };

template <C>
constexpr bool is_c() { return true; }

static_assert(is_c<void>(), "");

int main() {}
