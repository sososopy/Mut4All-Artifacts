
using A = decltype([](int){ return 1; });
using B = decltype([](){ return 2; });

struct S : A, B {
    using A::operator();
};

int main() {
    S s;
    static_assert( s() == 2 ); // erroneously passes in GCC and Clang
}
