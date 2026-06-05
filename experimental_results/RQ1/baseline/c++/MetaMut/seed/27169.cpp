
    #include <type_traits>

    int main()
    {
        int x = 5;

        [&]() {
            decltype(auto) b = x;
            static_assert(std::is_same<decltype(b), int>::value, "types should match");
        };
    }

