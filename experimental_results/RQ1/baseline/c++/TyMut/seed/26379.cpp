
#include <array>
#include <string_view>

struct Currency {
    // commenting-in the `i` below fixes, as does passing a `const char *` to the ctor
    // int i{};
    constexpr explicit Currency(std::string_view name) {}
    // constexpr explicit Currency(const char *name) {}
};

static Currency get_c() {
    static constexpr std::array c{Currency{"BWP"}};
    return c[0];
}
