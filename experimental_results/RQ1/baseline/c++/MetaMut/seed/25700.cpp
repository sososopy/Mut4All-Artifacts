
template <int N>
struct Test {
    template <int M>
    friend void test(Test<M>& arg) noexcept(M == 0);
};

template <int N>
void test(Test<N>& arg) noexcept(N == 0) {}

int main() {
    Test<0> t;
    test(t);
    return 0;
}

