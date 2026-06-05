
#include <cstdint>

void foo(std::size_t &f) {
        f = 5;
}

struct A {
        uint32_t val;
};

int main() {
        A a, *aptr = &a;
        foo(aptr->val);
        return 0;
}

