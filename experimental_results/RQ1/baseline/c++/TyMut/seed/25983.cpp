
#include <cstdint>

uint8_t foo() {
    static constexpr uint8_t foo = 0xff;

    uint8_t ret = ~foo;

    return ret;
}

uint8_t bar() {
    static constexpr uint8_t foo = 0x00;

    uint8_t ret = ~foo;

    return ret;
}

int main() {
    foo();
    bar();
}

