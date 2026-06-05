template<typename T>
struct trait
{
  enum { value = 1 };
};

template<typename T>
concept foo = trait<T>::value;

static_assert(foo<int>);

