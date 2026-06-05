
#include <iostream>

constexpr int p(auto) { return 0; }
constexpr int q() { return p(0); }
constexpr int p(auto) requires true { return 1; }

static_assert(p(0) == 1);
static_assert(q() == 0);

int main()
{
    std::cout << q() << p(0) << '\n';
}
