

    template <typename T>
    class Foo {
    public:
        constexpr Foo(T k) : mK(k) {}

        constexpr friend bool operator==<T>(T lhs, const Foo& rhs);
    private:
        T mK;
    };

    template <typename T>
    constexpr bool operator==(T lhs, const Foo<T>& rhs) {
        return lhs == rhs.mK;
    }

    int main() {
        return 1 == Foo<int>(1) ? 0 : 1;
    }

