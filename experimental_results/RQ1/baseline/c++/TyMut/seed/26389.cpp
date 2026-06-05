
#include <string_view>
#include <iostream>
int main()
{
    using namespace std::literals;
    int x = 0;
    static_assert(std::is_void_v<std::decay_t<decltype(x)>>, "Cannot process pair."s);

    return 0;
}
