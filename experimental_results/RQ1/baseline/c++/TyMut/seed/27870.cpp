#include <cstddef>

class A {
public:
    char a;
};

class B : public A {
public:
    static constexpr size_t b = offsetof(B, a);
};

