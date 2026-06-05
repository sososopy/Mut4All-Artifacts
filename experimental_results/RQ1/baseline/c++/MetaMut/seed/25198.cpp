

    struct NonCopyable {
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;

        NonCopyable() {}

        ~NonCopyable() {} // to make it non-trivial
    };

    union A {
        A(): _a{} {}
        ~A() {}

        NonCopyable _a[5];
    } a;

    int main() {}

