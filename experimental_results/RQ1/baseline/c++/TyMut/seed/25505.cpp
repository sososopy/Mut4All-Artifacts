
template <int N>
struct print_arg {
  void print () { std::cout << N << '\n'; }
};

struct const_holder {
  static constexpr int CONSTANT = 42;
};

template <typename T>
struct identity {
  using type = T;
};

template <class T>
struct test_case : public identity<T> {
  using typename identity<T>::type;
  print_arg<type::CONSTANT> printer; // <- parse error
};

int main () {
  // Should print 42
  test_case<const_holder>().printer.print();
}

