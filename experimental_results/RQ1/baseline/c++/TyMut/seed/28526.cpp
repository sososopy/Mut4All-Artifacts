
#include <string_view>

constexpr auto g(auto name_fn)  {
    return name_fn();
}

template <char... Cs> struct ct_string {
    static constexpr char str[] = {Cs..., 0};
    static constexpr auto name() { return std::string_view{str, sizeof...(Cs)}; }
};

struct S : ct_string<'f', 'o', 'o'> {
    constexpr auto f() {
        return g(name);
    }
};

int main() {
    {
        S s;
        [[maybe_unused]] constexpr auto name = s.f();
    }

    return 0;
}
