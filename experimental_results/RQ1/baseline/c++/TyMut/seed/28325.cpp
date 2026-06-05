
#define FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

template <bool B>
struct bool_t { static constexpr bool value = B; };

template <typename T>
struct set {
  bool contains(T const&) const { return true; }
};

template <typename T>
void intersect(set<T> const& lhs, set<T> const& rhs) {
  auto f = [&]<typename T1>(T1&& _1) noexcept(decltype([] {
      return bool_t<noexcept(rhs.contains(_1))>{};
  }(FWD(_1)))::value) {
      return rhs.contains(_1);
  };
  f(1);
}

int main() 
{
    set<int> a, b;
    intersect(a, b);
}

