
class class0 {
  ~class0() noexcept {}
  friend class class1;
};
struct class1 {
  class0 field0;
  class1();
  ~class1();
};
template <typename T> using array = T[1]; 
template <int N>
auto f1() noexcept
  -> decltype(sizeof(array<class1>{}));

