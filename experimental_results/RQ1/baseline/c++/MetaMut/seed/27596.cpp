
#include <memory>

constexpr auto fun()
{
    int* i = new int{4};

    std::allocator<int> a{};
    a.deallocate(i, 1);

    return 0;
}

int main()
{
    constexpr auto f = fun();
}


