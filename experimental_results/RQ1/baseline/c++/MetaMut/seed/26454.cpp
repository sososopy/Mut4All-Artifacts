
    auto foo(auto a, auto... i) -> decltype(auto) {
        if constexpr (requires { a[i...]; }) {
            return a[i...];
        }
        else {
            return a;
        }
    }

    int x = foo(1);

