
template<typename T>
struct S4 {
  void foo() requires (*this) { }
};

static_assert((S4<int>{}.foo(), true));

