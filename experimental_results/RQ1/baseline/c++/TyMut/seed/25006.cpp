

    template<int... I>
    static constexpr bool f() { return (I < ...); }

    int main() {
        static_assert(f<0, 42, 3>(), "!");
    }

