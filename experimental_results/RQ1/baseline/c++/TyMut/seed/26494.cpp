
template <int T> struct variant {
  variant() noexcept(T>0) {}
};
template <int> 
struct m {
  variant<1> def{};
};
struct v {
  v(m<1>);
};
v t = {{}};
