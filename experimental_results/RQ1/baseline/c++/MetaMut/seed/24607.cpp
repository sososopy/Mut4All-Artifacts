
    int i = 0;
    const int j = 0;
    static_assert(std::is_same<decltype(true? i : j), int const &>::value, "");

