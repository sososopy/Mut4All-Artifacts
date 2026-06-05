
extern void foo(const int &) noexcept;
extern void cont() noexcept;
void test(int a) noexcept {
        foo(a);
        return cont();
}
