
template<int N>
struct foo {
    constexpr foo() : a() {}
    int a[N];
};

int main() {
    foo< (foo<1>{}).a[0] > f;
}

