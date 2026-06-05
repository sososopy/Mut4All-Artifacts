#include <stdexcept>
#include <utility>

constexpr int foo(bool v) { if (!v) throw std::runtime_error(""); return 10; }

static_assert(noexcept(foo(true)) == true, "");

int main() {
    int mass[foo(true)] = {};
    return mass[0];
}
