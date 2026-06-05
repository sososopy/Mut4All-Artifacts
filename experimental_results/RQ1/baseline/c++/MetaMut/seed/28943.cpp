
struct S {
    template<class=void>
    operator auto() const { return 2; }
};

int main() {
    S s;
    [[maybe_unused]] int d = s;
}
