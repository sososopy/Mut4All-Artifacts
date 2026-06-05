

    void f() noexcept {}

    void func(void(*ptr)() noexcept) {
        static_assert(noexcept(ptr()), "!");
    }

    int main() {
        func(&f);
    }


