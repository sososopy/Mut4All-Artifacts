
// g++ -std=c++1z test.cpp
template <bool Pred, class T> struct enable_if { typedef T type; };
template <class T> struct enable_if<false, T> {};

template <int Idx> struct blows_up { static_assert(Idx != Idx, ""); };

template <int Idx,
           // substitution should fail here
          typename enable_if<Idx != Idx, int>::type = 0,
          // GCC evaluates this statement
          class = typename blows_up<Idx>::type 
>
void Foo() {}

template <int Idx,
          // Substitution correctly fails here.
          class = typename enable_if<Idx != Idx, int>::type,
          class = typename blows_up<Idx>::type // OK. Not evaluated
>
constexpr void Bar() {}
// Check the constructor in as SFINAE context
template <int I> constexpr auto test(int) -> decltype((Foo<I>(), true)) { return true; }
template <int>   constexpr bool test(long) { return false; }

template <int I> constexpr auto test_bar(int) -> decltype((Bar<I>(), true)) { return true; }
template <int>   constexpr bool test_bar(long) { return false; }

static_assert(!test<3>(0), ""); // Blows up
static_assert(!test_bar<4>(0), ""); // OK.
