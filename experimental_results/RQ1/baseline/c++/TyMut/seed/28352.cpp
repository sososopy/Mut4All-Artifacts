struct empty {
  empty() = default;
  constexpr empty(int) { }
};

struct container {
  empty __begin_ = {};
  [[no_unique_address]] empty __size_ = 0;
};

constexpr bool test() {
  container s;
  return true;
}
static_assert(test());


