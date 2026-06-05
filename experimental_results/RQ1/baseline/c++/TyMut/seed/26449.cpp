
#include <iostream>
enum class Color { Red, Green, Blue };
auto operator<=>(Color lhs, Color rhs) = default;
int main() {
    return 0;
}
