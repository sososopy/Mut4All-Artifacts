
#include <initializer_list>

int f()
{
    return 0;
}

std::initializer_list<long (*)()> var = {f};
