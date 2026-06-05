
struct Foo {
    template <typename>
    void X() { }

    void test() {
        auto by_ref = [&](auto arg) {
            // anything that makes X<...> dependent will do
            X<decltype(arg)>();
        };
        by_ref(1);

        auto by_val = [=](auto arg) {
            X<decltype(arg)>();
        };
        by_val(1);
    }
};


int main() {
    Foo foo;
    foo.test();
}
