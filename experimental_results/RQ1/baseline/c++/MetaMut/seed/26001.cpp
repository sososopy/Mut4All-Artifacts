template<class, class> struct same       {static const bool value = false; };
template<class T>      struct same<T, T> {static const bool value = true;  };
template<class T> class id { typedef T type; };
static_assert( same<int, id<int>::type /* private */ >::value, "bad type");

