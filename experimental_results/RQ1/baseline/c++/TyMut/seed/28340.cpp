
#include <cstddef>

struct B {
    virtual ~B() = default;
};

struct D : B {};

struct C : B {};

bool foo(std::byte *p) {
    constexpr D obj;
    return __builtin_memcmp(p, &obj, sizeof(void *)) == 0;
}

