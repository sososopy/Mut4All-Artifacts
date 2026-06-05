
constexpr bool g() {
    int * p = nullptr;
    {
        int c = 0;
        p = &c;
    }
    return *p == 0; //indirection via dangling pointer
};

int main() {
    static_assert( g() );
}
