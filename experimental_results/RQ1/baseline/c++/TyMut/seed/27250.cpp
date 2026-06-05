
template <typename T, int N>
struct vector {
    union storage { 
        T t; 
        constexpr  storage() {}
    } data[N];
};

constexpr auto foo() {
    vector<char, 8> i;
    return i;
}
auto f = foo();
