
void test(int a, int* b) {
    *b = a;
}

template<typename ...argT>
inline void test_template(
    void (*func)(argT..., int*)
) {}

int main() {
    test_template<int>(test);
}

