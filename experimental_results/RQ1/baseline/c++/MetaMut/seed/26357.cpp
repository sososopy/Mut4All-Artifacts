
#include <utility>

struct A {
    int i_;

    A(int i) : i_(i_) { }
};

struct B {
    int i_;

    B(int i) : i_(std::move(i_)) { }
};

