
template <typename Type>
struct receiver {
  struct type;
};

struct test {
    void operator()(){}
};

template <typename Type>
struct receiver<Type>::type {

  template <typename... Values>
  using func = test;

  void cleanup() noexcept {
    func<>();
  }
};

