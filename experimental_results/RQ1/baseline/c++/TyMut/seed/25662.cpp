

template<int N> struct Int { static const int value = N; };

template<typename... T>
  using Count = Int<sizeof...(T)>;

template<typename... T>
struct test
{
  typedef Count< T... > count;

  static_assert( count::value == sizeof...(T), "clang can count" );
};

template struct test<int>;        // OK

template struct test<>;         // fails

template struct test<int, int>;     // fails
