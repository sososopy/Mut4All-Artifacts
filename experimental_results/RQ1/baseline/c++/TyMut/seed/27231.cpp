
template<bool, typename = void>
struct enable_if { };

template<typename T>
struct enable_if<true, T> {
  using type = T;
};

template<class T>
struct hard_error {
  static_assert(sizeof(T) == 0);
  static inline constexpr bool value = true;
};

template<class T>
struct always_false {
  static inline constexpr bool value = false;
};

template<class T>
int foo (int, typename enable_if<always_false<T>::value, int>::type = 0,
              typename enable_if<hard_error<T>::value, int>::type = 0 )
{
  return 0;
}

template<class T>
char const *foo (long)
{
  return "";
}

int
main ()
{
  char const *sz = foo<int>(0);
}
