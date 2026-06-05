
#include <initializer_list>
struct S {
    S() { __builtin_printf("ctor\n"); }
    ~S() { __builtin_printf("dtor\n"); }
};

template<typename ... Args>
void f(Args const & ... args) {
    for (auto const & s: { args... })
        ;
}

int main() {
    S s;
    f(s);
}

