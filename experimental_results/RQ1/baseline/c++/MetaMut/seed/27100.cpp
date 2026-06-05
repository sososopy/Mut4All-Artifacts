#include <optional>

int main() {
  std::optional<int> t = 2;
  using T = std::optional<int>;
  t.~T();
  return t.has_value();
}

