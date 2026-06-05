
#include <stdio.h>

template<bool LMode>
static auto TestFunc = [](int param1)
{
    return param1;
};

template<typename Func>
static void test(Func func)
{
    printf("%d\n", func(12345));
}

int main()
{
    test(TestFunc<false>);
    test(TestFunc<true>);
}
