template<bool B> struct bool_constant { static constexpr bool value = B; };

template<typename T>
  struct is_implicitly_default_constructible
  : bool_constant<requires (void (*f)(const T&)) { f({}); }>
  { };

class X {
  X();
public:
  X(int);
};

static_assert( !is_implicitly_default_constructible<X>::value );


