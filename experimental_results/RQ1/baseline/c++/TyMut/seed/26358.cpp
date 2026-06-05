
struct U {
    mutable int x = 2;
};

int main() {
    constexpr U u{};
    u.x = 1;
    static_assert( u.x == 2 ); // must fail, but ok in GCC
}

