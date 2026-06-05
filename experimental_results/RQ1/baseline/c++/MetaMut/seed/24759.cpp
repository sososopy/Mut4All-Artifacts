struct a {
    operator int();
};

template<typename A>
struct b {
    template<typename B, decltype(void(A{}.operator B()))* = nullptr>
    auto func() {}
};

int main() {
    b<a>{}.func<int>();
}