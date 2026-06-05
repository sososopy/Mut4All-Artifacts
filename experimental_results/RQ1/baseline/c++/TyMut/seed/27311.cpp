
struct A {
    A() = default;
    A(const A&) {}
};

void (*fptr)(A) = [](auto){};
