

#include <functional>
#include <optional>

enum class Color { Red, Green, Blue };
size_t load(size_t);

int main() {
  size_t currentValue = load(0);
  auto ready = [currentValue]() -> std::optional<Color> {
    if (load(1) != currentValue) {
      return Color::Red;
    }
    return std::optional<Color>();
  };
  std::function<std::optional<Color>()> temp(ready);
  (void)temp;
}

