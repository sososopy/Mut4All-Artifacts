
template <typename T> struct V {};
struct S { template<typename T> S &operator>>(T &t) = delete; };
template<typename T> S &operator>>(S &s, V<T> &v);
void f(S s, V<int> v) { s >> v; }

