
// Reproduce decltype bug, condensed from Spirit X3
// gcc 4.9.1 fails to identify f1's declaration
template <typename T>
struct s1;
template <typename T>
struct s2 {
  typedef s1<T> t1;
};
template <typename T>
struct s1 {
  template <typename T2>
  s2<T> operator = (T2 const &) const;
};

struct T;
s1<T> v1;
s2<T> v2;
template <typename T>
void f1(decltype(v1 = v2)::t1);

