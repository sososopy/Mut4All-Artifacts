int main() {
    struct A {
        int x, i = [&] { return x; }();
    } a{0};
}

