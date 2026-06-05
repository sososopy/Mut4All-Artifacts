
template<typename T>
struct traits
{
  static constexpr bool foo() { return sizeof(T) > 1; }
  static constexpr bool bar() { return sizeof(T) > 1; }
};

template<typename T>
struct X
{
  X& operator=(X&&) noexcept(traits<T>::foo());
};

template<typename T>
  X<T>&
  X<T>::operator=(X&&) noexcept(traits<T>::foo() && traits<T>::bar())
  { return *this; }


