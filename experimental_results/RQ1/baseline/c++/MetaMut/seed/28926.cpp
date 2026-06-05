template<int I> struct S { };

template<typename T, T...> struct integer_sequence { };

template<typename T, T N>
  using make_integer_sequence
#if __has_builtin(__make_integer_seq)
    = __make_integer_seq<integer_sequence, T, N>;
#else
    = integer_sequence<T, __integer_pack(N)...>;
#endif

template<typename... _Types>
  concept trivially_destructible
    = (__has_trivial_destructor(_Types) && ...);

template<typename...> union variadic_union { static constexpr int size = 0; };

template<typename T, typename... U>
union variadic_union<T, U...>
{
  ~variadic_union() = default;

#ifndef TRIVIAL_ONLY
  // Conditionally non-trivial dtor, if required.
  constexpr ~variadic_union() requires (!trivially_destructible<T, U...>)
  { }
#endif

  T first;
  variadic_union<U...> rest;

  static constexpr int size = variadic_union<U...>::size + 1;
};


template <int... Is>
void f_impl(integer_sequence<int, Is...>)
{
  using V = variadic_union<S<Is>...>;
  // cause instantiation of V:
  static_assert( V::size == sizeof...(Is) );
}

  template <int I>
void f()
{
  f_impl(make_integer_sequence<int, I>());
}

int main()
{
  f<254>();
  f<255>();
  f<256>();
}

