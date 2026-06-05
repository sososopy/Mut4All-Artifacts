
template <bool B>
struct bool_constant { static constexpr bool value = B; };
template <typename T, typename... Args>
struct is_constructible : bool_constant<__is_constructible(T, Args...)> {};
template <typename T>
T&& move(T&);

struct X {
  template <typename OtherFunc>
  requires(is_constructible<OtherFunc, OtherFunc>::value)
  X(OtherFunc &&);

  X() = default;
};

X source;
X dest = move(source);
