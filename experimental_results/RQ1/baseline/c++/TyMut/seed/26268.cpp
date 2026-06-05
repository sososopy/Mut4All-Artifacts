
template<typename T, T...N> struct integer_sequence { typedef T value_type; };

template<typename T> struct __make_integer_sequence;
template<typename T, T N> using make_integer_sequence = typename __make_integer_sequence<T>::template make<N, N % 2>::type;

template<typename T, typename T::value_type ...Extra> struct __make_integer_sequence_impl;
template<typename T, T ...N, T ...Extra> struct __make_integer_sequence_impl<integer_sequence<T, N...>, Extra...> {
  typedef integer_sequence<T, N..., sizeof...(N) + N..., Extra...> type;
};

template<typename T> struct __make_integer_sequence {
  template<T N, T Parity, typename = void> struct make;
  template<typename Dummy> struct make<0, 0, Dummy> { typedef integer_sequence<T> type; };
  template<typename Dummy> struct make<1, 1, Dummy> { typedef integer_sequence<T, 0> type; };
  template<T N, typename Dummy> struct make<N, 0, Dummy> : __make_integer_sequence_impl<make_integer_sequence<T, N/2>> {};
  template<T N, typename Dummy> struct make<N, 1, Dummy> : __make_integer_sequence_impl<make_integer_sequence<T, N/2>, N> {};
};

using X = make_integer_sequence<int, 5>;

