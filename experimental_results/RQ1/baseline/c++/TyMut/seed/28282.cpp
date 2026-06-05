
#include <iostream>
#include <vector>

struct S
{
    S(bool) {}
};

int main()
{
    std::vector<S> v = { true, false, true };
    std::cout << v.size() << std::endl;
}

