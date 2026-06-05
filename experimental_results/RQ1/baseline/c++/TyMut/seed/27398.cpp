
#include <algorithm>

int main()
{
    struct s { int m; };
    s r[] = { s{0}, s{1}, s{2}, s{3} };
    std::ranges::find_if(r, [](auto const) { return true; });
}
