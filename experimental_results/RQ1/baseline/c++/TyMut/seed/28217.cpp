
#include <initializer_list>

struct IntSimilar { constexpr IntSimilar(int) {} };

constexpr int f(int) { return 1; }
constexpr int f(std::initializer_list<IntSimilar>) { return 2; }

int main() {
    static_assert( f({0}) == 1 );
}
