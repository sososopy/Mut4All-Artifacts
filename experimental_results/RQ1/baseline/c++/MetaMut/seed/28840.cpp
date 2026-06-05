
  #include <ranges>
  #include <array>
  #include <stream>
  #include <span>
  int main() {
    auto ints = std::istringstream{"42"};
    auto r = std::ranges::istream_view<int>(ints);
    std::array<int, 1> arr{42};
    std::span p{arr};
    std::ranges::lazy_split_view s(r, p);
  }
