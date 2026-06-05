

void test() {
    constexpr int N = 1;
    constexpr auto a1 = [](auto){
        static_assert(N == 1); //OK
        return 2;
    }(3);
    constexpr auto a2 = [=](auto){
        static_assert(N == 1); //OK
        return 2;
    }(3);
    constexpr auto a3 = [&](int){
        static_assert(N == 1); //OK
        return 2;
    }(3);
    constexpr auto a4 = [&](auto){
        static_assert(N == 1); //ICE in tsubst_copy, at cp/pt.c:16780
        return 2;
    }(3);
}
