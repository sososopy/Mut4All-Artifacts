
#include <type_traits>
struct A {
    constexpr static int a = 17;
};

struct B {
    A val;
    int foo() {
        // gcc compiles without warning; clang rejects
        return std::integral_constant<int, val.a>::value;
    }
    int good_foo() {
        // gcc compiles without warning, correct
        return std::integral_constant<int, decltype(val)::a>::value;
    }
    void bar() {
        char a[val.a]; // gcc compiles without warning, clang warns -Wvla
        (void) a;
    }
    void var_bar(int len) {
        char a[len]; // gcc compiles with -Wvla, clang does as well
        (void) a;
    }
};

int main() {}

