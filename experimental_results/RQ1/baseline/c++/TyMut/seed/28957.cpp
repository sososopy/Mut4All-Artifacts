
import <variant>;

int main() {
  std::variant<int, char> a;
  std::visit([](auto&&) {}, a);
  return 0;
}

