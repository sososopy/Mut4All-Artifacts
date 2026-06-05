
template <class T, class U> concept C = requires(T t, U u) { t * u; };
template <class Rep> struct Int {
     template <class T> requires C<T, Rep> friend void operator*(T, Int) { }
     template <class T> requires C<T, Rep> friend void operator*(Int, T) { }
};
void f() { 0 * Int<int>{}; }
