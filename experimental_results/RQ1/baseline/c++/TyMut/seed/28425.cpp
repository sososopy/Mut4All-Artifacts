struct A{
    constexpr operator int(){
        return 8;
    }
};
struct B{
    int a:A{};
};

