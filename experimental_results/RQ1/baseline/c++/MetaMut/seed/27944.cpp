
    constexpr int f() {
        struct {
            mutable int i = 41;
        } s;
        auto const& cs = s;
        return ++cs.i;
    }

    int main() {
        constexpr int i = f();
        return 42 - i;
    }
