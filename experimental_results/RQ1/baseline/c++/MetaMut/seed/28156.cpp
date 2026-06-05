
#include <array>

class Foo { public: Foo() {} int i; };

int main() {
 std::array<Foo, 1000000> arr = {{}}; // Halting problem.
}
