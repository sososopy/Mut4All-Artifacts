
    #include <bit>
    union U { int u; };
    consteval void f() {
        U result[]{0, 0};   
        auto x = std::bit_cast<long>(result);
    }
    int main() { f(); }

