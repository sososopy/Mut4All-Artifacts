

struct test
{
    int m;
    
    test() = default;
    test(int) : test() {}
};

#include <iostream>

int main()
{
    test t(0);
    std::cout << t.m;
}

