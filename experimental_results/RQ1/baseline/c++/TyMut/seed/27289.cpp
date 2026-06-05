
  #include <compare>
  #include <functional>

  struct S {
    // making it a member fixes the issue
    friend auto operator<=>(S, S) = default;
  };

  // removing 'rhs' fixes the issue
  template <typename Lhs, typename Rhs>
  auto operator<(Lhs&&, Rhs&& rhs) {
    return true;
  }

  int main() {
    // using operator< directly fixes the issue
    //return S{} < S{};
    return std::less{}(S{}, S{});
  }

