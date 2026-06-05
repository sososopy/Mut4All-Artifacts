

// following libstdc++ std::atomic
template <typename T>
struct my_atomic {
  T n;
  my_atomic() = default;
  explicit constexpr my_atomic(T n) : n(n) {}
  T load() const { return n; }
};

inline
my_atomic<int>&
a1() {
  static thread_local my_atomic<int> v;
  return v;
}

inline
my_atomic<int>&
a2() {
  static thread_local my_atomic<int> v{0};
  return v;
}

int foo() {
  return a1().load() + a2().load();
}


