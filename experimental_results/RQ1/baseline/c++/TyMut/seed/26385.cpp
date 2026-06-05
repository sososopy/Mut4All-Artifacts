
#include <cstdint>

struct Empty {};

struct S : Empty {
    uint32_t Uint32;
};

void consume(const S&) {}

void test() {
    S s = { .Uint32 = 42 };
    consume(s);
}
