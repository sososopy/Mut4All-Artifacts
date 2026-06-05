
template <typename>
struct Cls {
  void fun() override { }
};

nor does:

template <int>
struct Cls {
  void fun() final { }
};
