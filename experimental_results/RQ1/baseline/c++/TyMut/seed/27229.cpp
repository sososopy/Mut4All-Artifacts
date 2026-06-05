
template <auto F>
    requires requires { F(); }
bool v{};

void f() {
    int x;
    static_assert(v<throw>);
}
