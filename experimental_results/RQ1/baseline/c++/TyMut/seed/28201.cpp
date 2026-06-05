
constexpr bool test() { 
    int i[2] {};
    int j[2] {};
    return i+0 == j+1;
}
constexpr bool x = test();

