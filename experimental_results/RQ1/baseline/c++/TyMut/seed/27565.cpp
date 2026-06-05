
template<typename T>
  using blah = T::type;

template<typename T>
  concept C = requires (T t) { t + 0; };

template<typename T>
  concept D = C<T> || C<const T>;

template<typename T>
  concept E = D<blah<T>>;

static_assert(E<int>);
