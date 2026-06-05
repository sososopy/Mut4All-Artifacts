
#include <range/v3/all.hpp>

struct Wrapper {
    int a[1];

    constexpr operator int() const { return a[0]; }
};

int main() {
    std::vector<int> a;
    auto all = ranges::views::all(a);
    auto zip = ranges::views::zip(a);

    Wrapper b;
    *all.begin() = b;
    *zip.begin() = b;
}
