template<typename T> struct A
{
  static constexpr bool value = true;

  explicit operator bool() const { return value; }
};

template<typename T> concept foo = A<T>() || A<T>();

static_assert(foo<int>);


