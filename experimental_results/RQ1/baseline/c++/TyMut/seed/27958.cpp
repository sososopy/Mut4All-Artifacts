
    template <typename X = int, typename T, typename ...R>
        inline void func(const T&, R...) {}

    template <typename T>
        struct S {};

    template <typename X = int, typename T, typename ...R>
        inline void func(const S<T>&, R...) {}

    int main() {
        func(42);
        func(S<int>()); // OK
        func(S<int>(), 1); // NOK
        func<int>(S<int>(), 1); // NOK
    }

