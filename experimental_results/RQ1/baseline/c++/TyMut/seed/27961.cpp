
// works
template<typename... Ts>
struct X {};

template<typename T, typename... Rest>
struct X<T, Rest...> {};


// works
template<int... Is>
struct Y {};

template<int I, int... Rest>
struct Y<I, Rest...> {};


// error
struct A {
  int value;
  // auto operator<=>(const A&) = default;
};

template<A... Us>
struct Z {};

template<A V, A... Rest>
struct Z<V, Rest...> {};
