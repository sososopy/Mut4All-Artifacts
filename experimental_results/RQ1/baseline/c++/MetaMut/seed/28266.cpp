
template<typename ...T> struct X {};
template<typename ...T> int f1(X<T, T...>... a);

int a1 = f1(X<int, int, double>(), X<double, int, double>());

