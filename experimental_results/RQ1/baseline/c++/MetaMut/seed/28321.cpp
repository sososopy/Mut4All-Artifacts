

template<class>
concept C = [] { return true; } ();

template<class T> // <-- ok if not 'T'
struct S {};

template<class T>
  requires C<T>
struct S<T> { constexpr static bool value = true; };

static_assert(S<int>::value);

