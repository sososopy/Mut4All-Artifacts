
template<typename T> struct tuple { tuple(T); };
template<typename T> explicit tuple(T t) -> tuple<T>;
tuple t = { 1 };

