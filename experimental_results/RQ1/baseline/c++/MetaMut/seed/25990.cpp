struct test {
    test() {}
    static const int value = 10;
};

template<int N>
struct cst {};

int main(int argc, const char* argv[]) {
    test t;
    test& rt = t;

    cst<t.value> v; // works
    int arr[rt.value]; // works
    cst<rt.value> rv; // compile error

    return 0;
}
