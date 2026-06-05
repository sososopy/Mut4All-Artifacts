
namespace std {
template <class> struct initializer_list {
  int *_M_array;
  unsigned long _M_len;
};
struct c {
  c(int);
  ~c();
};
struct d {
  d(initializer_list<c>);
};
struct e {
  d f{0};
};
template <typename> void h() {
  e {};
};
}