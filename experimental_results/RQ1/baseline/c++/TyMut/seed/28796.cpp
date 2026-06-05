
#include <variant>
#include <string_view>

struct my_variant : std::variant<std::string_view> { };
int main() {
  std::string_view s = "hello";
  my_variant v{s};
}

