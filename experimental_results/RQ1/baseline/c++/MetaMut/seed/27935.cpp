template<typename T, typename U> constexpr bool foo = true;

template<typename T> int f(T) requires foo<T> { return 0; }

int i = f(0);


