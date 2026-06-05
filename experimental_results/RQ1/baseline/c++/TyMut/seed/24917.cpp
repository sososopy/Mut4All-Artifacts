#include <type_traits>

template <auto& Value, int>
struct test : std::false_type {};

template <auto& Value>
struct test<Value, 0> : std::true_type {};

int main() {
    static constexpr int v = 42;
    static_assert(test<v, 0>::value); // error ?!
}