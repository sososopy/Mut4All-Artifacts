
    #include <variant>
    #include <string>
    #include <vector>

    using variant = std::variant<
        std::string,
        std::vector<char>
    >;

    extern std::size_t &ext_index;

    void func() {
        ext_index = variant(std::string()).index();
    }

