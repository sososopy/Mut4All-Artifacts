
template<typename = void> auto f() {return 0U;}
template<typename = void>
struct B {
    unsigned g() {
        auto i = 0U, j = f();
        return i+j;
    }
};
auto i = B<>{}.g();