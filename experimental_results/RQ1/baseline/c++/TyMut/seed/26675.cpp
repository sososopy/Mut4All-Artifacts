
template <typename> struct A;
template <bool> struct enable_if;
template <typename T, T> struct B;
template <bool... Bools> using and_c = A<B<bool, Bools || true...>>;
template <typename...> using Constructible = int;
template <typename... Ts> struct common_tuple {
  template <
      typename... Us,
      typename enable_if<and_c<(int)Constructible<Ts, Us>()...>::value>::type>
};

